/*
 * fita.h
 *
 * Created: 03/06/2024 15:00:36
 *  Author: Cleiton
 */ 


#ifndef FITA_H_
#define FITA_H_

#include "serial.h"

void fita_begin();
void fita_send_frame();
void fita_load_send_rgb_position(uint8_t r, uint8_t g, uint8_t b, int led_position);
void fita_send_black_all();
void fita_send_rgb_all(uint8_t r, uint8_t g, uint8_t b);

#endif /* FITA_H_ */