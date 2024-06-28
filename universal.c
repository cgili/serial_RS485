#include "teste/universal.h"




struct {
    uint8_t rx1; // Temporary RX
    bool new_data[NUM_BUFFERS]; // Flags to indicate new data is available in each buffer
    serial_packet_t packet_rx[NUM_BUFFERS]; // Array of packets for the buffers
    int write_idx; // Index to write to the next packet
    int read_idx; // Index to read the next packet
} va;

bool serial_wait_response(serial_packet_t *data)
{
    if (va.read_idx != va.write_idx) { // Check if there is data to be processed
        if (va.new_data[va.read_idx]) {
            memcpy(data, &va.packet_rx[va.read_idx], sizeof(serial_packet_t));
            va.new_data[va.read_idx] = false; // Clear the data ready flag
            va.read_idx = (va.read_idx + 1) % NUM_BUFFERS; // Move to the next buffer
            return true;
        }
    }
    return false;
}


// static uint16_t serial_crc_calc(serial_packet_t *packet)
// {
//     uint16_t crc = 0xFFFF;
//     int size = packet->size + 4; // Inclui type, addr, cmd e size

//     // Acessa os bytes do pacote como um array de bytes
//     uint8_t *packet_bytes = (uint8_t *)packet;

//     for (int i = 0; i < size; i++) {
//         crc ^= packet_bytes[i];
//         for (int j = 0; j < 8; j++) {
//             if (crc & 0x0001) {
//                 crc >>= 1;
//                 crc ^= 0xA001;
//                 } else {
//                 crc >>= 1;
//             }
//         }
//     }
//     return crc;
// }