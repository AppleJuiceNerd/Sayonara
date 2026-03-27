// C/C++ stuff
#include <cstdio>

// imgui
#include "imgui.h"

// Nara
#include <naraapi.h>

// GUI stuff
#include "gui.h"

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

