#include "circular_buffer.h"

void circ_buffer_reset(circ_buffer_t* buffer){
    uint8_t i;
    for(i = 0; i < BUFFER_SIZE; i++){
        buffer->memory[i] = 0;
    }
    buffer->head = 0;
    buffer->tail = BUFFER_SIZE + 1;
}

void circ_buffer_add_value(circ_buffer_t* buffer, uint16_t value){
}

uint8_t circ_buffer_read_value(circ_buffer_t* buffer, uint16_t* value){
}
