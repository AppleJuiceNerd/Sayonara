#include <stdint.h>
#include <hidapi/hidapi.h>


// SayoDevice API Structures
struct API_LED_DATA
{
	uint8_t led_mode;
	uint8_t event;
	uint8_t lighting_time;
	uint8_t dark_time;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t color_table_number;
};


#define CMD_0X11_SIZE 56

// NOTE: All of these fields, barring led_fn, have unknown purposes.
#pragma pack(1)
struct API_CMD_0X11 {
	uint8_t valid;
	uint8_t led_class;
	uint16_t reserve1;
	uint16_t led_site_x;
	uint16_t led_site_y;
	uint16_t led_width;
	uint16_t led_height;
	uint16_t fillet_angle;
	uint16_t reserve2;
	struct API_LED_DATA led_fn[5]; // 5 fns
};

struct PKT_HEADER {
	uint8_t report_id;
	uint8_t echo;
	uint16_t checksum;
};

struct CMD_HEADER {
	uint16_t data_length;
	uint8_t command;
	uint8_t index;
};


// Nara Macros

// Nara's Echo (mostly functionally useless, just cool to have)
#define NARA_ECHO_CODE 0xC5


namespace Nara 
{
	// Lower level functions
	namespace LL
	{
		uint16_t checksum(uint8_t *data, int length);
		void set_key_lights(hid_device *sayo, uint8_t key, struct API_CMD_0X11 req_data, uint8_t *result);
		void read_key_lights(hid_device *sayo, uint8_t key, uint8_t *result);
	};

	// Higher level functions and interfaces

	// Represents a color
	typedef struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	} Color;

	// Represents a Sayodevice.
	class Sayo
	{
	private:
		hid_device *device;
	public:
		// Gets the first sayodevice found and uses it
		Sayo();

		// Getter for the write hid device
		hid_device *get_device();

		
		void SetLight(int key, int fn, Color color);
		Color ReadLight(int key, int fn);

	};

	// Basically wraps hid_init() to a T. Returns 0 on success, -1 otherwise.
	int Init();

	// Basically wraps hid_exit() to a T. Returns 0 on success, -1 otherwise.
	int Exit();

	// Returns a list of all of the Sayodevices that are connected
	// This doesn't exist yet due to limited testing resources
	// void Devices();
}

