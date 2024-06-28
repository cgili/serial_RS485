#include <sus.h>
#include "universal.h"


void send_sus()
{
    serial_packet_t packet;
    packet.type = DEVICE_TYPE_SUS;
    packet.addr = 1;
    packet.cmd = READ_SENSOR;
    serial_write_data(&packet);
}



