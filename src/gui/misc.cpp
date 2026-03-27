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