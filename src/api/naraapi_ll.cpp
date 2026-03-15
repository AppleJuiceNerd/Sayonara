#include "naraapi.h"
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