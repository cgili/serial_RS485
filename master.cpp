#include "master.h"
#include "universal.h"


struct {
    uint8_t rx1; // Temporary RX
    bool new_data[NUM_BUFFERS]; // Flags to indicate new data is available in each buffer
    serial_packet_t packet_rx[NUM_BUFFERS]; // Array of packets for the buffers
    int write_idx; // Index to write to the next packet
    int read_idx; // Index to read the next packet
} va;


void serial_begin(void)
{
    Serial.begin(115200);
    Serial1.begin(115200);
}

bool serial_tx_busy() {
    // Verifica se o buffer de transmissão está disponível
    return !Serial1.availableForWrite();
}

bool serial_write_data(serial_packet_t *packet) {
    static uint8_t tx_buf[MAX_RX_BUFFER_SIZE + 3];
    uint16_t crc = 0;

    for (int i = 0; i < 100; i++) {
        //  if (!serial_tx_busy()) 
        //  {
            tx_buf[0] = 'M';
            crc = serial_crc_calc(packet);
            tx_buf[1] = crc >> 8;
            tx_buf[2] = crc & 0xFF; // Correção aqui para garantir que só os 8 bits menos significativos sejam usados
            // Copia os bytes do pacote para o buffer de transmissão
            memcpy(&tx_buf[3], packet, packet->size + 4);

            // Escreve o buffer na UART
            //digitalWrite(RTS_PIN, HIGH); // Habilita transmissão
            Serial.print("Sent packet: ");
            for (int j = 0; j < packet->size + 4 + 3; j++) {
                Serial.print(tx_buf[j]);
                Serial.print(" ");
            }
            Serial.println();
            Serial1.write(tx_buf, packet->size + 4 + 3);
            Serial.println(packet->size + 4 + 3);
            //digitalWrite(RTS_PIN, LOW); // Desabilita transmissão

            return true;
        //  }
        delayMicroseconds(100);
    }
    return false;
}


static void IRQ_RX_CALLBACK() {
	static uint8_t idx = 0;
	static uint8_t size = 0;
	static uint16_t crc = 0;
	static bool reset = false;

	if (!va.new_data[va.write_idx]) {  
		    serial_packet_t *current_packet = &va.packet_rx[va.write_idx];

		
		switch (idx) 
		{
			case 0:
			if (va.rx1 == 'S') {idx++;} 
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
					va.write_idx = (va.write_idx + 1) % NUM_BUFFERS;  
				}
				else {reset = true;}
				idx = 0;
			}
			break;
		}
	}

	if (reset) 
	{
		//serial_reset();
		reset = false;
		idx = 0;
	}
}


void serialEvent() {
    while (Serial.available()) {
        va.rx1 = Serial.read(); 
        IRQ_RX_CALLBACK(); 
    }
}

