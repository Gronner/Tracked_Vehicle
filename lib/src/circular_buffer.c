#include "circular_buffer.h"

static uint8_t increment(uint8_t value) {
    value++;
    if(value > BUFFER_SIZE) {
        value = 0;
    }
    return value;
}

static uint8_t tail_update(circ_buffer_t* buffer) {
    if (buffer->tail == buffer->head) {
        return 1;
    }
    buffer->tail = increment(buffer->tail);
    return 0;
}

static void head_update(circ_buffer_t* buffer) {
    buffer->head= increment(buffer->head);
    if(buffer->head == buffer->tail) {
        buffer->tail = increment(buffer->tail);
    }
}

void circ_buffer_reset(circ_buffer_t* buffer) {
    uint8_t i;
    for (i = 0; i < BUFFER_SIZE + 1; i++) {
        buffer->memory[i] = 0;
    }
    buffer->head = 0;
    buffer->tail = 0;
}

void circ_buffer_add_value(circ_buffer_t* buffer, uint16_t value) {
    buffer->memory[buffer->head] = value;
    head_update(buffer);
}

uint8_t circ_buffer_read_value(circ_buffer_t* buffer, uint16_t* value) {
    *value = buffer->memory[buffer->tail];
    return tail_update(buffer);
}
