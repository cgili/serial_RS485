/*
 * fita.h
 *
 * Created: 03/06/2024 15:00:36
 *  Author: Cleiton
 */ 


#ifndef FITA_H_
#define FITA_H_

#include <stdint.h> 

void fita_begin();
void fita_send_frame();
void fita_load_send_rgb_position(uint8_t r, uint8_t g, uint8_t b, int led_position);
void fita_send_black_all();
void fita_send_rgb_all(uint8_t r, uint8_t g, uint8_t b);



enum fita_led_cmd
{
	FITA_LED_CMD_SET_RGB_ALL = 1,	// Define 3 bytes de rgb para toda a fita
	FITA_LED_CMD_SET_BLACK_ALL,		// Apaga toda a fita
	FITA_LED_CMD_LOAD_RGB_POSITION,	// Envia uma cor respecífica para um frame_buffer, mas não envia para fita
	FITA_LED_CMD_SEND_FRAME			// Mostra na fita o buffer pre carregado
};


#endif /* FITA_H_ */