// C/C++ stuff
#include <format>

// imgui
#include "imgui.h"

// Nara
#include <naraapi.h>

// GUI stuff
#include "gui.h"


#define WIDGET_WIDTH ( ImGui::GetContentRegionAvail().x * 0.3f )


void array_combo(const char *label, const char **items, int *selected, int size)
{
	// Combo element
	ImGui::SetNextItemWidth(WIDGET_WIDTH);
	if (ImGui::BeginCombo(label, items[*selected]))
	{
		for (int i = 0; i < size; i++)
		{
			const bool is_selected = (*selected == i);
			
			// If the current item is selected, set the selected item index to the current item's
			if (ImGui::Selectable(items[i], is_selected)) 
			{
				*selected = i;
			}

			// Set initial focus when opening the combo
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

Nara::LED_Modes led_mode_switcher(Nara::Sayo *sayo, int key, int fn)
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

	// The last key passed to this function
	static int last_key = -1; // -1 to force a reread

	// The last fn layer passed to this function
	static int last_fn = -1; // -1 to force a reread


	// If the key or fn passed isn't the same as the one that was last passed
	if ((last_key != key) || (last_fn != fn))
	{
		last_key = key;
		last_fn = fn;

		switch (sayo->ReadLightMode(key, fn))
		{
			case Nara::STATIC:
				selected = 0;
				break;
			
			case Nara::INDICATOR:
				selected = 1;
				break;
			
			case Nara::BREATHING:
				selected = 2;
				break;
			
			case Nara::BREATHING_ONCE:
				selected = 3;
				break;
			
			case Nara::WAVE:
				selected = 4;
				break;
			
			case Nara::SWITCH:
				selected = 5;
				break;
			
			case Nara::SWITCH_ONCE:
				selected = 6;
				break;
			
			case Nara::BLINK:
				selected = 7;
				break;
			
			case Nara::BLINK_ONCE:
				selected = 8;
				break;
			
			case Nara::FADE_IN:
				selected = 9;
				break;
			
			case Nara::FADE_OUT:
				selected = 10;
				break;
		}
	}

	array_combo("Light Mode", items, &selected, IM_COUNTOF(items));

	// Evaluate selected element and send the corresponding configuration command when changed
	if (selected != last_selected)
	{
		last_selected = selected;

		switch (selected)
		{
			case 0:
				sayo->SetLightMode(key, fn, Nara::STATIC);
				break;

			case 1:
				sayo->SetLightMode(key, fn, Nara::INDICATOR);
				break;
			
			case 2:
				sayo->SetLightMode(key, fn, Nara::BREATHING);
				break;
			
			case 3:
				sayo->SetLightMode(key, fn, Nara::BREATHING_ONCE);
				break;
			
			case 4:
				sayo->SetLightMode(key, fn, Nara::WAVE);
				break;
			
			case 5:
				sayo->SetLightMode(key, fn, Nara::SWITCH);
				break;
			
			case 6:
				sayo->SetLightMode(key, fn, Nara::SWITCH_ONCE);
				break;
			
			case 7:
				sayo->SetLightMode(key, fn, Nara::BLINK);
				break;
			
			case 8:
				sayo->SetLightMode(key, fn, Nara::BLINK_ONCE);
				break;
			
			case 9:
				sayo->SetLightMode(key, fn, Nara::FADE_OUT);
				break;
			
			case 10:
				sayo->SetLightMode(key, fn, Nara::FADE_IN);
				break;
		}
	}

	return sayo->ReadLightMode(key, fn);
}

Nara::LED_ColorModes color_mode_switcher(Nara::Sayo *sayo, int key, int fn)
{
	// All possible options
	const char* items[] = {
		"Static Color",
		"Loop Color Tables",
		"Random Color Tables",
		"Random Color"
	};

	// The currently selected item
	static int selected = 0;

	// The last selected item
	static int last_selected = 0;

	// The last key passed to this function
	static int last_key = -1; // -1 to force a reread

	// The last fn layer passed to this function
	static int last_fn = -1; // -1 to force a reread


	// If the key or fn passed isn't the same as the one that was last passed
	if ((last_key != key) || (last_fn != fn))
	{
		last_key = key;
		last_fn = fn;

		selected = sayo->ReadColorMode(key, fn);
	}

	array_combo("Color Mode", items, &selected, IM_COUNTOF(items));

	// Evaluate selected element and send the corresponding configuration command when changed
	if (selected != last_selected)
	{
		last_selected = selected;

		sayo->SetColorMode(key, fn, (Nara::LED_ColorModes) selected);
	}

	return sayo->ReadColorMode(key, fn);
}

int color_table_switcher(Nara::Sayo *sayo, int key, int fn)
{
	// All possible options
	// NOTE: This list should probably be generated instead
	const char* items[] = {
		"Color Table 1",
		"Color Table 2",
		"Color Table 3",
		"Color Table 4",
		"Color Table 5",
		"Color Table 6",
	};

	// The currently selected item
	static int selected = 0;

	// The last selected item
	static int last_selected = 0;

	// The last key passed to this function
	static int last_key = -1; // -1 to force a reread

	// The last fn layer passed to this function
	static int last_fn = -1; // -1 to force a reread


	// If the key or fn passed isn't the same as the one that was last passed
	if ((last_key != key) || (last_fn != fn))
	{
		last_key = key;
		last_fn = fn;

		selected = sayo->GetLightColorTable(key, fn);
	}

	array_combo("Color Table", items, &selected, IM_COUNTOF(items));

	// Evaluate selected element and send the corresponding configuration command when changed
	if (selected != last_selected)
	{
		last_selected = selected;

		sayo->SetLightColorTable(key, fn, selected);
	}

	return sayo->GetLightColorTable(key, fn);
}

int trigger_event_switcher(Nara::Sayo *sayo, int key, int fn)
{
	// All possible options
	// NOTE: This list should probably be generated instead
	const char* items[] = {
		"None",
		"Press Light Up",
		"Release Light Up",
		"Press Fade In",
		"Release Fade In",
		"Press Next Color",
		"Release Next Color",
		"Script_Running",
		"Script_Stop"
	};

	// The currently selected item
	static int selected = 0;

	// The last selected item
	static int last_selected = 0;

	// The last key passed to this function
	static int last_key = -1; // -1 to force a reread

	// The last fn layer passed to this function
	static int last_fn = -1; // -1 to force a reread


	// If the key or fn passed isn't the same as the one that was last passed
	if ((last_key != key) || (last_fn != fn))
	{
		last_key = key;
		last_fn = fn;


		selected = sayo->GetLightTriggerEvent(key, fn);
	}

	array_combo("Trigger Event", items, &selected, IM_COUNTOF(items));

	// Evaluate selected element and send the corresponding configuration command when changed
	if (selected != last_selected)
	{
		last_selected = selected;

		sayo->SetLightTriggerEvent(key, fn, selected);
	}

	return sayo->GetLightTriggerEvent(key, fn);
}
}

void color_picker(Nara::Color *in_color)
{
	static float color[3] = { 0 };
	
	color[0] = (in_color->r / 255.0f);
	color[1] = (in_color->g / 255.0f);
	color[2] = (in_color->b / 255.0f);

	ImGui::SetNextItemWidth(WIDGET_WIDTH);
	ImGui::ColorPicker3("Light Color", color, 0);

	in_color->r = (uint8_t)(color[0] * 255);
	in_color->g = (uint8_t)(color[1] * 255);
	in_color->b = (uint8_t)(color[2] * 255);
}

void led_picker(Nara::Sayo *sayo, Nara::Color *color, int buttons, int *key, int fn)
{
	for (int i = 0; i < buttons; i++)
	{
		if (ImGui::RadioButton(std::format("light {}", i + 1).c_str(), key, i))
		{
			// Click a button, get that button's color
			*color = sayo->ReadLight(*key, fn);
		}
		ImGui::SameLine();
	}

	// The loop will always end with SameLine();, this fixes that
	ImGui::NewLine();
}


void color_config(Nara::Sayo *sayo)
{
	// The color to be sent to the device
	static Nara::Color color = { 0 };

	// The light to send the color to
	static int btn_number = 0;

	// The fn layer to send the color to
	static int fn = 0;

	// The number of buttons [to assume] that are on the device
	int btns = 3;

	// Color Picker
	color_picker(&color);

	// Light picker
	led_picker(sayo, &color, btns, &btn_number, fn);
	
	// Light effects
	led_mode_switcher(sayo, btn_number, fn);
	color_mode_switcher(sayo, btn_number, fn);
	color_table_switcher(sayo, btn_number, fn);
	trigger_event_switcher(sayo, btn_number, fn);

	ImGui::NewLine();

	if (ImGui::Button("Send", ImVec2(80, 50)))
	{
		sayo->SetLight(btn_number, fn, {color});
	}
}

