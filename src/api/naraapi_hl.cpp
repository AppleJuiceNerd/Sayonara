#include "naraapi.h"
#include <cstring>
#include <hidapi.h>


int Nara::Init()
{
	return hid_init();
}

int Nara::Exit()
{
	return hid_exit();
}

// FIXME: This function might be prone to errors. I don't have more than one device to test.
Nara::Sayo::Sayo()
{
	struct hid_device_info *devices;
	
	// SayoDevice vendor code is 0x8089
	devices = hid_enumerate(0x8089, 0x0);

	// Loop through all found devices
	devices = devices->next; // should get the other path, which is writable
	device = hid_open_path(devices->path);
	
	hid_free_enumeration(devices);
}

hid_device *Nara::Sayo::get_device()
{
	return device;
}

void Nara::Sayo::SetLight(int key, int fn, Color color)
{
	// The data to be sent and received
	uint8_t results[1024] = { 0 };

	// Get current state of the lights
	LL::read_key_lights(device, key, results);

	// Copy that state to a struct
	API_CMD_0X11 lights = { 0 };
	memcpy(&lights, &results[8], sizeof(lights));

	// Modify the color
	lights.led_fn[fn].r = color.r;
	lights.led_fn[fn].g = color.g;
	lights.led_fn[fn].b = color.b;

	// Set the new lights
	LL::set_key_lights(device, key, lights, results);
	return;
}

Nara::Color Nara::Sayo::ReadLight(int key, int fn)
{
	// The data to be sent and received
	uint8_t results[1024] = { 0 };

	// Get current state of the lights
	LL::read_key_lights(device, key, results);

	// Copy that state to a struct
	API_CMD_0X11 lights = { 0 };
	memcpy(&lights, &results[8], sizeof(lights));

	// Return the color
	return {
		lights.led_fn[fn].r,
		lights.led_fn[fn].g,
		lights.led_fn[fn].b
	};
}