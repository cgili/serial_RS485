/*
 * gerador.h
 *
 * Created: 12/03/2021 14:39:24
 *  Author: Cleiton
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#include <asf.h>
#include <string.h>
#include <stdint.h>







#define MAX_RX_BUFFER_SIZE 8
#define NUM_BUFFERS 4


enum device_type
{
	DEVICE_TYPE_FITA_LED = 'L',
};


enum fita_led_cmd
{
	FITA_LED_CMD_SET_RGB_ALL = 1,	// Define 3 bytes de rgb para toda a fita
	FITA_LED_CMD_SET_BLACK_ALL,		// Apaga toda a fita
	FITA_LED_CMD_LOAD_RGB_POSITION,	// Envia uma cor respecífica para um frame_buffer, mas não envia para fita
	FITA_LED_CMD_SEND_FRAME			// Mostra na fita o buffer pre carregado
};



typedef struct
{
	uint8_t type;
	uint8_t addr;
	uint8_t cmd;
	uint8_t size;
	uint8_t data[MAX_RX_BUFFER_SIZE];
} serial_packet_t;




void serial_begin(void);
bool serial_write_data(serial_packet_t * packet);
bool serial_wait_response(serial_packet_t * packet);
bool serial_tx_busy(void);



#endif /* SERIAL_H_ */