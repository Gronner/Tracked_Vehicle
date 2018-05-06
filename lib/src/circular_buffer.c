#include "circular_buffer.h"

void circ_buffer_reset(circ_buffer_t* buffer){
    uint8_t i;
    for(i = 0; i < BUFFER_SIZE; i++){
        buffer->memory[i] = 0;
    }
    buffer->head = 0;
    buffer->tail = 0;
}
