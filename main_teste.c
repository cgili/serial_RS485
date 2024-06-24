#include <asf.h>
#include "serial.h"



int main (void) {
	system_init();
	delay_init();
	system_interrupt_enable_global();
	fita_begin();
		
	
	while (1) {
			
		// exemplo fita inteira + black	
		fita_send_rgb_all(25,0,0);
		delay_ms(500);		
		fita_send_rgb_all(0,0,25);
		delay_ms(500);		
		fita_send_rgb_all(0,25,0);
		delay_ms(500);	
		fita_send_black_all();
		delay_ms(1000);
		
		
		// exemplo polícia
		// fita_load_send_rgb_position(25,0,0,0);
		// fita_load_send_rgb_position(25,0,0,1);
		// fita_load_send_rgb_position(25,0,0,2);
		// fita_load_send_rgb_position(0,0,25,3);
		// fita_load_send_rgb_position(0,0,25,4);
		// fita_load_send_rgb_position(0,0,25,5);
		// fita_send_frame();
		// delay_ms(300);
		
		// fita_load_send_rgb_position(25,0,0,3);
		// fita_load_send_rgb_position(25,0,0,4);
		// fita_load_send_rgb_position(25,0,0,5);
		// fita_load_send_rgb_position(0,0,25,0);
		// fita_load_send_rgb_position(0,0,25,1);
		// fita_load_send_rgb_position(0,0,25,2);
		// fita_send_frame();
		// delay_ms(300);
		

	
	}
}
