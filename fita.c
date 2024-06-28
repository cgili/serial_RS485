/*
 * fita.c
 *
 * Created: 03/06/2024 15:00:18
 *  Author: Cleiton
 */ 

#include "teste/fita.h"



void fita_send_rgb_all(uint8_t r, uint8_t g, uint8_t b)
{
	serial_packet_t packet;
	packet.type = DEVICE_TYPE_FITA_LED;
	packet.addr = 1;
	packet.cmd = FITA_LED_CMD_SET_RGB_ALL;
	packet.size = 3;
	packet.data[0] = r;
	packet.data[1] = g;
	packet.data[2] = b;
	serial_write_data(&packet);
}

void fita_send_black_all()
{
	serial_packet_t packet;
	packet.type = DEVICE_TYPE_FITA_LED;
	packet.addr = 1;
	packet.cmd = FITA_LED_CMD_SET_BLACK_ALL;
	packet.size = 1;
	serial_write_data(&packet);
}

void fita_load_send_rgb_position(uint8_t r, uint8_t g, uint8_t b, int led_position)
{
	serial_packet_t packet;
	packet.type = DEVICE_TYPE_FITA_LED;
	packet.addr = 1;
	packet.cmd = FITA_LED_CMD_LOAD_RGB_POSITION;
	packet.size = 4;
	packet.data[0] = r;
	packet.data[1] = g;
	packet.data[2] = b;
	packet.data[3] = led_position;
	serial_write_data(&packet);
}



void fita_send_frame()
{
	serial_packet_t packet;
	packet.type = DEVICE_TYPE_FITA_LED;
	packet.addr = 1;
	packet.cmd = FITA_LED_CMD_SEND_FRAME;
	packet.size = 1;
	serial_write_data(&packet);
}


void fita_begin()
{
	serial_begin();	
}