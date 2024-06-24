 #include <serial.h>
 

 static struct usart_module serial;
 

struct {
	uint8_t rx1;                    // Temporary RX
	bool new_data[NUM_BUFFERS];     // Flags to indicate new data is available in each buffer
	serial_packet_t packet_rx[NUM_BUFFERS]; // Array of packets for the buffers
	int write_idx;                  // Index to write to the next packet
	int read_idx;                   // Index to read the next packet
} va;


bool serial_tx_busy()
{
	enum status_code status = usart_get_job_status(&serial, USART_TRANSCEIVER_TX);
	if (status != STATUS_OK) {return true;}
	return false;
}


bool serial_wait_response(serial_packet_t *data) {
	if (va.read_idx != va.write_idx) {  // Check if there is data to be processed
		if (va.new_data[va.read_idx]) {
			memcpy(data, &va.packet_rx[va.read_idx], sizeof(serial_packet_t));
			va.new_data[va.read_idx] = false;  // Clear the data ready flag
			va.read_idx = (va.read_idx + 1) % NUM_BUFFERS;  // Move to the next buffer
			return true;
		}
	}
	return false;
}


static uint16_t serial_crc_calc(serial_packet_t *packet) {
	uint16_t crc = 0xFFFF;
	int size = packet->size + 4; // Inclui type, addr, cmd e size

	// Acessa os bytes do pacote como um array de bytes
	uint8_t *packet_bytes = (uint8_t *)packet;

	for (int i = 0; i < size; i++) {
		crc ^= packet_bytes[i];
		for (int j = 0; j < 8; j++) {
			if (crc & 0x0001) {
				crc >>= 1;
				crc ^= 0xA001;
				} else {
				crc >>= 1;
			}
		}
	}
	return crc;
}





 void serial_reset()
 {
	 usart_abort_job(&serial, USART_TRANSCEIVER_RX);
	 delay_ms(1);
	 usart_disable(&serial);	// So funcionou assim
	 usart_enable(&serial);
 }

static void IRQ_RX_CALLBACK() {
	static uint8_t idx = 0;
	static uint8_t size = 0;
	static uint16_t crc = 0;
	static bool reset = false;

	if (!va.new_data[va.write_idx]) {  // Only write to buffer if it's not marked as containing new data
		    serial_packet_t *current_packet = &va.packet_rx[va.write_idx];

		
		switch (idx) 
		{
			case 0:
			if (va.rx1 == 'M') {idx++;} 
			else {reset = true;}
			break;
			
			case 1: crc = va.rx1 << 8; idx++; break;
			
			case 2: crc |= va.rx1; idx++; break;
			
			case 3: current_packet->type = va.rx1; idx++; break;
			
			case 4: current_packet->addr = va.rx1; idx++; break;
			
			case 5: current_packet->cmd = va.rx1; idx++; break;
			
			case 6:
			size = va.rx1; current_packet->size = size; idx++;
			if (size > MAX_RX_BUFFER_SIZE) {reset = true;}	
			break;
			
			
			default:
			if (idx < size + 7) {current_packet->data[idx - 7] = va.rx1; idx++;}
			if (idx >= size + 7) 
			{
				uint16_t calculated_crc = serial_crc_calc(current_packet);
				
				if (crc == serial_crc_calc(current_packet)) {
					va.new_data[va.write_idx] = true;
					va.write_idx = (va.write_idx + 1) % NUM_BUFFERS;  // Move to the next buffer
				}
				else {reset = true;}
				idx = 0;
			}
			break;
		}
	}

	if (reset) 
	{
		serial_reset();
		reset = false;
		idx = 0;
	}

	usart_read_job(&serial, &va.rx1);
}




bool serial_write_data(serial_packet_t *packet) {
	static uint8_t tx_buf[MAX_RX_BUFFER_SIZE+3];
	uint16_t crc = 0;

	for (int i = 0; i < 100; i++) {
		if (usart_get_job_status(&serial, USART_TRANSCEIVER_TX) == STATUS_OK) {
			tx_buf[0] = 'M';
			crc = serial_crc_calc(packet);
			tx_buf[1] = crc >> 8;
			tx_buf[2] = crc & 0xFF; // Correção aqui para garantir que só os 8 bits menos significativos sejam usados
			// Copia os bytes do pacote para o buffer de transmissão
			memcpy(&tx_buf[3], packet, packet->size + 4);

			// Escreve o buffer na USART
			usart_write_buffer_job(&serial, tx_buf, packet->size + 4+3);
			return true;
		}
		delay_us(100);
	}
	return 0;
}





//___________________________________________________________________________________________
void serial_begin()
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.generator_source = GCLK_GENERATOR_0;
	config_usart.baudrate    = 115200;
	config_usart.mux_setting = SERIAL_MUX_SETTINGS;
	config_usart.pinmux_pad0 = SERIAL_PAD0_MUX;
	config_usart.pinmux_pad1 = SERIAL_PAD1_MUX;
	config_usart.pinmux_pad2 = SERIAL_PAD2_MUX;
	config_usart.pinmux_pad3 = SERIAL_PAD3_MUX;
	

	while (usart_init(&serial, SERIAL, &config_usart) != STATUS_OK) {}
	usart_enable(&serial);
	
	usart_register_callback(&serial, IRQ_RX_CALLBACK, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&serial, USART_CALLBACK_BUFFER_RECEIVED);
	usart_read_job(&serial, &va.rx1);
}


