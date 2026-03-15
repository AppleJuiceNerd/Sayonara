#include <cstdio>
#include <format>

#include <hidapi/hidapi.h>

#include "raylib.h"

#include "imgui.h"
#include "rlImGui.h"	// include the API header

#include "api/naraapi.h"
#include "utils.h"


void about_window()
{
	
	ImGui::Begin("About", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
	
		
	
	

	ImGui::Text("Sayonara Driver");
	ImGui::Text("Made by AppleJuiceNerd");
	if (ImGui::TreeNode("Tools and frameworks used"))
	{
		ImGui::BulletText("Dear ImGui");
		ImGui::BulletText("Raylib");
		ImGui::BulletText("rlImGui");
		ImGui::BulletText("hidapi");
		ImGui::TreePop();
	}
	


	ImGui::End();
}

void sn_window()
{
	// Setup device
	static Nara::Sayo sayo = Nara::Sayo();

	// Make this window fullscreen
	static bool use_work_area = false;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	// whether the about window is open or not
	static bool about_window_open = false;

	// The color to be sent to the device
	static float color[3] = { 0 };

	// The button to send the color to
	static int btn_number = 0;

	// The number of buttons [to assume] that are on the device
	int btns = 3;

	// Do not overwrite the starting value color
	Nara::Color col = sayo.ReadLight(btn_number, 0);
	color[0] = (col.r / 255.0f);
	color[1] = (col.g / 255.0f);
	color[2] = (col.b / 255.0f);

	ImGuiWindowFlags window_flags = 
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus;


	// Start drawing the window
	ImGui::SetNextItemAllowOverlap();
	if (ImGui::Begin("SayoNara", NULL, window_flags))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("About", NULL, &about_window_open);
				ImGui::EndMenu();
			}
			
		}
		ImGui::EndMenuBar();

		// Color Picker
		ImGui::SetNextItemWidth(300);
		ImGui::ColorPicker3("Light Color", color, 0);
		
		// Light picker
		for (int i = 0; i < btns; i++)
		{
			if (ImGui::RadioButton(std::format("light {}", i).c_str(), &btn_number, i))
			{
				// Click a button, get that button's color
				// FIXME: Does not work compiled on anything other than GCC
				// TODO: Pass Fn
				Nara::Color col = sayo.ReadLight(btn_number, 0);
				color[0] = (col.r / 255.0f);
				color[1] = (col.g / 255.0f);
				color[2] = (col.b / 255.0f);

				
			}

			ImGui::SameLine();
			
		}

		ImGui::NewLine();
		
		
		// send the color to the device!
		// setup a color
		Nara::Color pkt_color = {
			(unsigned char)(color[0] * 255.0f),
			(unsigned char)(color[1] * 255.0f),
			(unsigned char)(color[2] * 255.0f)
		};

		// TODO: Pass Fn
		sayo.SetLight(btn_number, 0, pkt_color);





	}
	
	
	ImGui::End();
	if (about_window_open) { about_window(); }

}


int main()
{
	// Setup HID things
	Nara::Init();

	struct hid_device_info *devices;
	devices = hid_enumerate(0x8089, 0x0);
	print_devices(devices);
	hid_free_enumeration(devices);

	// Setup window + imgui things
	Vector2 aspect_ratio = {3, 2};
	int mul = 400;

	InitWindow(aspect_ratio.x * mul, aspect_ratio.y * mul, "Sayonara");
	SetTargetFPS(60);

	rlImGuiSetup(true);

	while(!WindowShouldClose())
	{
		

		BeginDrawing();
		ClearBackground(BLACK);

		rlImGuiBegin();
		
		sn_window(); 

		rlImGuiEnd();
;

		EndDrawing();
	}


	// Clean up
	Nara::Exit();
	rlImGuiShutdown();

	return 0;
}
