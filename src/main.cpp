// raylib
#include "raylib.h"

// imgui
#include "rlImGui.h"

// Nara
#include <naraapi.h>

// Sayonara stuff
#include "gui/gui.h"


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
