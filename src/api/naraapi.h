#include <stdint.h>
#include <hidapi/hidapi.h>
#include <vector>


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
// #pragma pack(1)
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
		// Represents a SayoDevice command package
		// Acts as a base class
		class Package
		{
		protected:
			uint8_t command; // Configured based on derived class
			uint16_t length; // Also configured based on derived class

		public:
			uint8_t index;

			// Assembles the package into a byte array for use in a Packet
			virtual void GetBytes(uint8_t *bytes);

			// Reads a Package and loads corresponding fields
			virtual void LoadBytes(uint8_t *bytes);

			// Gets the length of this Package
			virtual uint16_t GetLength();
		};

		class LightData : public Package
		{
		public:
			// NOTE: All of these fields, barring led_fn, have unknown purposes.
			uint8_t valid         = 0;
			uint8_t led_class     = 0;
			uint16_t reserve1     = 0;
			uint16_t led_site_x   = 0;
			uint16_t led_site_y   = 0;
			uint16_t led_width    = 0;
			uint16_t led_height   = 0;
			uint16_t fillet_angle = 0;
			uint16_t reserve2     = 0;
			struct API_LED_DATA led_fn[5] = { 0 }; // 5 fns


			// Assembles the package into a byte array for use in a Packet
			void GetBytes(uint8_t *bytes) override;

			// Assembles the package into a byte array for use in a Packet
			void LoadBytes(uint8_t *bytes) override;
			

			uint16_t GetLength() override;

			// Set private fields
			LightData(bool read = false)
			{
				command = 0x11;
				if(!read)
				{
					length = 56; // LightData packages are always 56 bytes long
				}
				else 
				{
					length = 0;
				}
			}
		};

		// Represents a SayoDevice HID packet
		class Packet
		{
		public:
			bool long_packet = true; // Determines if the packet length is 64 (false) or 1024 (true)
			uint8_t echo = NARA_ECHO_CODE; // The echo code

			std::vector<Package*> packages; // The packages that make up the packet

			// Assembles the packet into a byte array for sending
			void GetBytes(uint8_t *bytes);
		};

		
		// A deserialization helper for SayoDevice communication packets
		class Boxcutter
		{
		private:
			uint8_t data[1024] = { 0 };
			std::vector<uint16_t> offsets; 

		public:
			Boxcutter(uint8_t *bytes);

			// Gets the offset at index
			uint16_t GetOffset(int index);

		};
		
		uint16_t checksum(uint8_t *data, int length);
		void set_key_lights(hid_device *sayo, uint8_t key, LightData req_data, uint8_t *result);
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

