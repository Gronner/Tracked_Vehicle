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

/*! @brief Resets the circular buffer to an empty state
 * @buffer Pointer to the buffer structure to be reset
 */
void circ_buffer_reset(circ_buffer_t* buffer);

/*! @brief Add a value into the ring buffer
 * @param Pointer to the target buffer
 * @param value Value to be inserted
 */
void circ_buffer_add_value(circ_buffer_t* buffer, uint16_t value);

/*! @brief Read out a value from the buffer.
 * @param buffer Pointer to the source buffer
 * @param value Object where the read out value should be stored
 * @return 0 if sucessfull, 1 if buffer is empty
 */
uint8_t circ_buffer_read_value(circ_buffer_t* buffer, uint16_t* value);
#endif  // __CIRC_BUFFER_H
