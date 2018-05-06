#ifndef __CIRC_BUFFER_H
#define __CIRC_BUFFER_H
#include <stdint.h>

#define BUFFER_SIZE 16

typedef struct {
    uint16_t memory[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
} circ_buffer_t;

void circ_buffer_reset(circ_buffer_t* buffer);
#endif // __CIRC_BUFFER_H
