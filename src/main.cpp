// C/C++ stuff
#include <cstdio>
#include <format>

// raylib
#include "raylib.h"

// imgui
#include "imgui.h"
#include "rlImGui.h"

// Sayonara stuff
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

void device_not_found_window()
{
	
	ImGui::Begin("Device Not Found", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text("There was no SayoDevice found. Do you have one connected?");
	ImGui::Text("Make sure your SayoDevice is connected and restart the application.");

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

	// Specifies whether the about window is open or not
	static bool about_window_open = false;

	// The color to be sent to the device
	static float color[3] = { 0 };

	// The light to send the color to
	static int btn_number = 0;

	// The number of buttons [to assume] that are on the device
	int btns = 3;

	// Do not overwrite the starting value color
	if (sayo.get_device() != NULL)\
	{
		Nara::Color col = sayo.ReadLight(btn_number, 0);
		color[0] = (col.r / 255.0f);
		color[1] = (col.g / 255.0f);
		color[2] = (col.b / 255.0f);
	}
	

	// Window flags
	ImGuiWindowFlags window_flags = 
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus;


	// Start drawing the window
	ImGui::SetNextItemAllowOverlap();
	if (ImGui::Begin("SayoNara", NULL, window_flags) && sayo.get_device() != NULL)
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
		// NOTE: a color is being sent every frame. Maybe it shouldn't do this?
		// setup a color
		Nara::Color pkt_color = {
			(unsigned char)(color[0] * 255.0f),
			(unsigned char)(color[1] * 255.0f),
			(unsigned char)(color[2] * 255.0f)
		};

		// Send color
		// TODO: Pass Fn
		sayo.SetLight(btn_number, 0, pkt_color);
	}
	else if (sayo.get_device() == NULL)
	{
		device_not_found_window();
	}
	
	
	ImGui::End();

	// Draw the about window if it's open
	if (about_window_open) { about_window(); }

}


int main()
{
	// Setup Nara
	Nara::Init();

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
			
				sn_window(); // Sayonara's window

			rlImGuiEnd();

		EndDrawing();
	}


	// Clean up
	Nara::Exit();
	rlImGuiShutdown();

	return 0;
}
