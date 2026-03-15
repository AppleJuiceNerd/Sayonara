#include <hidapi/hidapi.h>

const char *hid_bus_name(hid_bus_type bus_type);
void print_device(struct hid_device_info *cur_dev);
void print_devices(struct hid_device_info *cur_dev);