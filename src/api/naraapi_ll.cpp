#include "naraapi.h"
#include <cstdint>
#include <hidapi.h>
#include <string.h>

// NOTE: While it may not happen, this function may not work properly with odd-length arrays.
uint16_t Nara::LL::checksum(uint8_t *data, int length)
{
	uint16_t sum = 0;
	for(int b = 0; b < length; b+=2)
	{
		sum += ( data[b+1] << 8 ) + data[b];
		// printf("off %d: %d\n", b, sum);
	}

	return sum;
}

void Nara::LL::set_key_lights(hid_device *sayo, uint8_t key, struct API_CMD_0X11 req_data, uint8_t *result)
{
	struct PKT_HEADER pkt = { 0x22, NARA_ECHO_CODE, 0 };
	struct CMD_HEADER cmd = { 60, 0x11, key };
	uint8_t data[1024] = { 0 };

	memcpy(&data, &pkt, sizeof(pkt));
	memcpy(&data[4], &cmd, sizeof(cmd));
	memcpy(&data[8], &req_data, sizeof(req_data));

	// pkt.checksum = __builtin_bswap16(checksum(data, 1000));
	pkt.checksum = checksum(data, 1000);
	
	memcpy(&data, &pkt, sizeof(pkt));

	hid_write(sayo, data, 1024);
	

	// NOTE: May be prone to infinite loops
	do {
		hid_read(sayo, data, 1024);
	} while (data[1] != NARA_ECHO_CODE);

	memcpy(&data, &result, 1024);
}

void Nara::LL::read_key_lights(hid_device *sayo, uint8_t key, uint8_t *result)
{
	struct PKT_HEADER pkt = { 0x22, NARA_ECHO_CODE, 0 };
	struct CMD_HEADER cmd = { 0x04, 0x11, key };
	uint8_t data[1024] = { 0 };

	memcpy(&data, &pkt, sizeof(pkt));
	memcpy(&data[4], &cmd, sizeof(cmd));

	// pkt.checksum = __builtin_bswap16(checksum(data, 1000));
	pkt.checksum = checksum(data, 1000);
	
	memcpy(&data, &pkt, sizeof(pkt));
	

	hid_write(sayo, data, 1024);

	// NOTE: May be prone to infinite loops
	do {
		hid_read(sayo, data, 1024);
	} while (data[1] != NARA_ECHO_CODE);
	

	memcpy(result, &data, 1024);
}


// Package Building

void Nara::LL::Package::GetBytes(uint8_t *bytes)
{
	; // Nothing should happen here
}

uint16_t Nara::LL::Package::GetLength()
{
	return length;
}


// LightData

uint16_t Nara::LL::LightData::GetLength()
{
	return length;
}

void Nara::LL::LightData::GetBytes(uint8_t *bytes)
{
	// Header
	*(uint16_t*)(&bytes[0]) = length + 4;
	bytes[2] = command;
	bytes[3] = index;

	// Data
	bytes[4] = valid;
	bytes[5] = led_class;
	*(uint16_t*)(&bytes[6]) = reserve1;
	*(uint16_t*)(&bytes[8]) = led_site_x;
	*(uint16_t*)(&bytes[10]) = led_site_y;
	*(uint16_t*)(&bytes[12]) = led_width;
	*(uint16_t*)(&bytes[14]) = led_height;
	*(uint16_t*)(&bytes[16]) = fillet_angle;
	*(uint16_t*)(&bytes[18]) = reserve2;

	for(int i = 0; i < 5; i++)
	{
		memcpy(&bytes[20 + (i * 8)], &led_fn[i], 8);
	}
}

void Nara::LL::Packet::GetBytes(uint8_t *bytes)
{
	int size;

	// Check if the packet should be long 
	if (long_packet)
	{
		size = 1024;
		bytes[0] = 0x22;
	} else
	{
		size = 64;
		bytes[0] = 0x21;
	}

	bytes[1] = echo;


	// setup loop
	// offset in the packet
	int offset = 4;

	// The bytes that will be filled by the packages' GetBytes function
	
	uint8_t buffer[1024] = { 0 };

	for(int pkg = 0; pkg < packages.size(); pkg++)
	{
		// Assemble package bytes
		packages[pkg]->GetBytes(buffer);

		// Copy package to bytes at offset
		memcpy(&bytes[offset], &buffer, packages[pkg]->GetLength());

		// Move to the next available space
		// There should be four zeros after each package
		offset += packages[pkg]->GetLength() + 4;
	}

	*(uint16_t*)(&bytes[2]) = checksum(bytes, size);
}


// Boxcutter

Nara::LL::Boxcutter::Boxcutter(uint8_t *bytes)
{
	// Copy the passed bytes to data
	if(bytes[0] == 0x21)
	{
		memcpy(data, bytes, 64);
	} else
	{
		memcpy(data, bytes, 1024);
	}

	// Look for packages in data

	int offset = 4;

	// If the value at the 
	while(bytes[offset] != 0)
	{
		offsets.push_back(offset);
		offset += *(uint16_t*)(&bytes[offset]);
	}
}