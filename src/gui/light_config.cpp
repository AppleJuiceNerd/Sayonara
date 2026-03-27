// C/C++ stuff
#include <format>

// imgui
#include "imgui.h"

// Nara
#include <naraapi.h>

// GUI stuff
#include "gui.h"


void led_mode_switcher(Nara::Sayo *sayo, int key, int fn)
{
	// All possible options
	const char* items[] = {
		"Static",
		"Indicator",
		"Breathing",
		"Breathing Once",
		"Wave",
		"Switch",
		"Switch Once",
		"Blink",
		"Blink Once",
		"Fade Out",
		"Fade In"
	};

	// The currently selected item
	static int selected = 0;

	// The last selected item
	static int last_selected = 0;


	// Combo element
	if (ImGui::BeginCombo("Light Mode1", items[selected]))
	{
		for (int i = 0; i < IM_COUNTOF(items); i++)
		{
			const bool is_selected = (selected == i);
			
			// If the current item is selected, set the selected item index to the current item's
			if (ImGui::Selectable(items[i], is_selected)) 
			{
				selected = i;
			}

			// Set initial focus when opening the combo
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();


		// Evaluate selected element and send the corresponding configuration command when changed
		if (selected != last_selected)
		{
			last_selected = selected;

			switch (selected)
			{
				case 1:
					sayo->SetLightMode(key, fn, Nara::STATIC);

				case 2:
					sayo->SetLightMode(key, fn, Nara::INDICATOR);
				
				case 3:
					sayo->SetLightMode(key, fn, Nara::BREATHING);
				
				case 4:
					sayo->SetLightMode(key, fn, Nara::BREATHING_ONCE);
				
				case 5:
					sayo->SetLightMode(key, fn, Nara::WAVE);
				
				case 6:
					sayo->SetLightMode(key, fn, Nara::SWITCH);
				
				case 7:
					sayo->SetLightMode(key, fn, Nara::SWITCH_ONCE);
				
				case 8:
					sayo->SetLightMode(key, fn, Nara::BLINK);
				
				case 9:
					sayo->SetLightMode(key, fn, Nara::BLINK_ONCE);
				
				case 10:
					sayo->SetLightMode(key, fn, Nara::FADE_OUT);
				
				case 11:
					sayo->SetLightMode(key, fn, Nara::FADE_IN);
			}
		}
	}
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

	led_mode_switcher(sayo, btn_number, fn);

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

