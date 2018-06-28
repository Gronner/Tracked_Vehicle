#ifndef __CIRC_BUFFER_H
#define __CIRC_BUFFER_H
#include <stdint.h>

#define BUFFER_SIZE 16

typedef struct {
  uint16_t memory[BUFFER_SIZE + 1];
  uint8_t head;
  uint8_t tail;
  uint8_t state;
} circ_buffer_t;

void circ_buffer_reset(circ_buffer_t* buffer);
void circ_buffer_add_value(circ_buffer_t* buffer, uint16_t value);
uint8_t circ_buffer_read_value(circ_buffer_t* buffer, uint16_t* value);
#endif  // __CIRC_BUFFER_H
