#include "circular_buffer.h"

void circ_buffer_reset(circ_buffer_t* buffer){
    uint8_t i;
    for(i = 0; i < BUFFER_SIZE; i++){
        buffer->memory[i] = 0;
    }
    buffer->head = 0;
    buffer->tail = 0;
}

void circ_buffer_add_value(circ_buffer_t* buffer, uint16_t value){
    buffer->memory[buffer->head] = value;
    buffer->head++;
    if(!(buffer->head < BUFFER_SIZE)){
        buffer->head = 0;
    }
}
