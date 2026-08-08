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

	gui_setup();

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
		// ImGui::RadioButton("Color Tables", &sidebar_window, 1);

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

