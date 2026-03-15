#include "utils.h"
#include <stdio.h>


const char *hid_bus_name(hid_bus_type bus_type) {
		static const char *const HidBusTypeName[] = {
				"Unknown",
				"USB",
				"Bluetooth",
				"I2C",
				"SPI",
		};
 
		if ((int)bus_type < 0)
				bus_type = HID_API_BUS_UNKNOWN;
		if ((int)bus_type >= (int)(sizeof(HidBusTypeName) / sizeof(HidBusTypeName[0])))
				bus_type = HID_API_BUS_UNKNOWN;
 
		return HidBusTypeName[bus_type];
}

void print_device(struct hid_device_info *cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("  Usage (page): 0x%hx (0x%hx)\n", cur_dev->usage, cur_dev->usage_page);
		printf("  Bus type: %u (%s)\n", (unsigned)cur_dev->bus_type, hid_bus_name(cur_dev->bus_type));
		printf("\n");
}

void print_devices(struct hid_device_info *cur_dev) {
		for (; cur_dev; cur_dev = cur_dev->next) {
				print_device(cur_dev);
		}
}