// C/C++ stuff
#include <cstdint>
#include <cstdio>
#include <format>

// imgui
#include "imgui.h"

// Nara
#include <naraapi.h>

// GUI stuff
#include "gui.h"


void about_window(bool *p_open)
{
	ImGui::Begin("About", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);


	ImGui::Text("Sayonara Driver");
	ImGui::Text("Made by AppleJuiceNerd");

	ImGui::Separator();

	ImGui::Text("Tools and frameworks used:");

	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextLinkOpenURL("Dear ImGui", "https://github.com/ocornut/imgui");

	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextLinkOpenURL("Raylib", "https://www.raylib.com/");

	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextLinkOpenURL("rlImGui", "https://github.com/raylib-extras/rlImGui");

	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextLinkOpenURL("hidapi", "https://github.com/libusb/hidapi");
		
	
	ImGui::End();
}

void device_not_found_window()
{
	
	ImGui::Begin("Device Not Found", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text("There was no SayoDevice found. Do you have one connected?");
	ImGui::Text("Make sure your SayoDevice is connected and restart the application.");

	ImGui::End();
}

void color_config(Nara::Sayo *sayo)
{
	// The color to be sent to the device
	static float color[3] = { 0 };

	// The light to send the color to
	static int btn_number = 0;

	// The fn layer to send the color to
	static int fn = 0;

	// The number of buttons [to assume] that are on the device
	int btns = 3;

	// Color Picker
	ImGui::SetNextItemWidth(300);
	ImGui::ColorPicker3("Light Color", color, 0);

	// Light picker
	for (int i = 0; i < btns; i++)
	{
		if (ImGui::RadioButton(std::format("light {}", i + 1).c_str(), &btn_number, i))
		{
			// Click a button, get that button's color
			// TODO: Pass Fn
			Nara::Color col = sayo->ReadLight(btn_number, fn);
			color[0] = (col.r / 255.0f);
			color[1] = (col.g / 255.0f);
			color[2] = (col.b / 255.0f);
		}
		ImGui::SameLine();
	}

	ImGui::NewLine();

	if (ImGui::Button("Send", ImVec2(80, 50)))
	{
		sayo->SetLight(btn_number, fn, {
			(uint8_t)(color[0] * 255),
			(uint8_t)(color[1] * 255),
			(uint8_t)(color[2] * 255)
		});
	}
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

	// Which sidebar window is selected
	static int sidebar_window = 0;

	ImGuiChildFlags sidebar_flags = 
		ImGuiChildFlags_Borders;
	
	// Window flags
	ImGuiWindowFlags window_flags = 
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	// Setup ImGui style
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 2;
	style.ChildRounding = 2;
	style.FrameRounding = 2;
	style.PopupRounding = 2;
	style.GrabRounding = 2;


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



		ImGui::BeginChild("Sidebar", ImVec2(ImGui::GetContentRegionAvail().x * 0.1f, ImGui::GetContentRegionAvail().y), sidebar_flags);

		ImGui::RadioButton("Lights", &sidebar_window, 0);
		ImGui::RadioButton("Keybinding", &sidebar_window, 1);

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginGroup();
		color_config(&sayo);
		ImGui::EndGroup();


	}

	else if (sayo.get_device() == NULL)
	{
		device_not_found_window();
	}
	
	
	ImGui::End();

	// Draw the about window if it's open
	if (about_window_open) { about_window(&about_window_open); }

}

