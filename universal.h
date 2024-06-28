
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//#include <asf>



#define MAX_RX_BUFFER_SIZE 8
#define NUM_BUFFERS 4

enum device_type
{
	DEVICE_TYPE_FITA_LED = 'L',
	DEVICE_TYPE_PUS = 'P',
	DEVICE_TYPE_SUS = 'S'
};


typedef struct
{
	uint8_t type;
	uint8_t addr;
	uint8_t cmd;
	uint8_t size;
	uint8_t data[MAX_RX_BUFFER_SIZE];
} serial_packet_t;






// static uint16_t serial_crc_calc(serial_packet_t *packet);
void serial_begin(void);
bool serial_write_data(serial_packet_t * packet);
bool serial_wait_response(serial_packet_t * packet);
bool serial_tx_busy(void);

