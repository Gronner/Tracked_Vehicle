#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include "circular_buffer.h"
    #include "string.h"
}

circ_buffer_t buffer;

TEST_GROUP(CircularBufferTestGroup){

    void setup(void){
    }

    void teardown(void){
    }
};

TEST(CircularBufferTestGroup, CircularBufferInitProperly){
    circ_buffer_reset(&buffer);

    uint16_t temp[BUFFER_SIZE + 1] = {}; // In C++ this sets all values to zero!
    // See if all values are inizialized to zero
    CHECK_EQUAL(0, memcmp(buffer.memory, temp, sizeof(buffer.memory)));
    CHECK_EQUAL(0, buffer.head); // Head should be at zero
    // Tail should be at the empty memory position
    CHECK_EQUAL(BUFFER_SIZE + 1, buffer.tail); 
}

TEST(CircularBufferTestGroup, CircularBufferResetProperly){
    for(uint8_t i = 0; i < BUFFER_SIZE; i++){
        buffer.memory[i] = i;
    }
    buffer.head = 12;
    buffer.tail = 8; 

    circ_buffer_reset(&buffer);

    uint16_t temp[BUFFER_SIZE + 1] = {}; // In C++ this sets all values to zero!
    CHECK_EQUAL(0, memcmp(buffer.memory, temp, sizeof(buffer.memory)));
    CHECK_EQUAL(0, buffer.head); // Head should be at zero
    CHECK_EQUAL(BUFFER_SIZE + 1, buffer.tail); // Tail should be at zero
}

TEST(CircularBufferTestGroup, CircularBufferAddValue){
    circ_buffer_reset(&buffer); // Reset for clean state
    uint16_t value = 1;

    circ_buffer_add_value(&buffer, value); 
    CHECK_EQUAL(value, buffer.memory[0]); 
    CHECK_EQUAL(1, buffer.head);
}

TEST(CircularBufferTestGroup, CircularBufferAddValues){
    circ_buffer_reset(&buffer); // Reset for clean state
    uint16_t value1 = 1;
    uint16_t value2 = 2;
    uint16_t value3 = 3;

    circ_buffer_add_value(&buffer, value1); 
    circ_buffer_add_value(&buffer, value2); 
    circ_buffer_add_value(&buffer, value3); 
    CHECK_EQUAL(value1, buffer.memory[0]); 
    CHECK_EQUAL(value2, buffer.memory[1]); 
    CHECK_EQUAL(value3, buffer.memory[2]); 
}

TEST(CircularBufferTestGroup, CircularBufferAddValuesFull){
    circ_buffer_reset(&buffer); // Reset for clean state
    uint16_t temp[BUFFER_SIZE] = {};
    for(uint16_t i = 0; i < BUFFER_SIZE; i++){
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }

    for(uint16_t i = 0; i < BUFFER_SIZE; i++){
        CHECK_EQUAL(temp[i], buffer.memory[i]);
    }
}

TEST(CircularBufferTestGroup, CircularBufferAddValuesOverfill){
    circ_buffer_reset(&buffer); // Reset for clean state
    uint16_t temp[BUFFER_SIZE+1] = {};
    for(uint16_t i = 0; i < BUFFER_SIZE + 1; i++){
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }
    
    CHECK_EQUAL(temp[BUFFER_SIZE], buffer.memory[0]);
    for(uint16_t i = 1; i < BUFFER_SIZE; i++){
        CHECK_EQUAL(temp[i], buffer.memory[i]);
    }
}

TEST(CircularBufferTestGroup, CircularBufferReadFirstValue){
    circ_buffer_reset(&buffer); // Reset for clean state
    // Fill with readable values
    uint16_t temp[BUFFER_SIZE] = {};
    for(uint16_t i = 0; i < BUFFER_SIZE; i++){
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }
    uint16_t value;
    uint8_t state;
    state = circ_buffer_read_value(&buffer, &value);

    CHECK_EQUAL_TEXT(temp[0], value, "Wrong value read");
    CHECK_EQUAL_TEXT(0, buffer.tail, "Tail at wrong position");
    CHECK_EQUAL_TEXT(0, state, "Wrong state returned");
}

TEST(CircularBufferTestGroup, CircularBufferReadValues){
    circ_buffer_reset(&buffer); // Reset for clean state
    // Fill with readable values
    uint16_t temp[BUFFER_SIZE+1] = {};
    for(uint16_t i = 0; i < BUFFER_SIZE; i++){
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }

    uint16_t value;
    uint8_t state;
    for(uint16_t i = 0; i < BUFFER_SIZE; i++){
        state = circ_buffer_read_value(&buffer, &value);
        CHECK_EQUAL_TEXT(0, state, "Wrong state returned");
        CHECK_EQUAL_TEXT(temp[i], value, "Wrong value read");
        CHECK_EQUAL_TEXT(i, buffer.tail, "Tail at wrong position");
    }
}

TEST(CircularBufferTestGroup, CircularBufferReadOverHead){
    circ_buffer_reset(&buffer); // Reset for clean state
    // Fill with readable values
    uint16_t temp[BUFFER_SIZE+1] = {};
    for(uint16_t i = 0; i < BUFFER_SIZE / 2; i++){
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }
    
    uint16_t value;
    uint8_t state;
    for(uint16_t i = 0; i < (BUFFER_SIZE / 2); i++){
        state = circ_buffer_read_value(&buffer, &value);
        CHECK_EQUAL_TEXT(0, state, "Wrong state returned");
        CHECK_EQUAL_TEXT(i, buffer.tail, "Tail at wrong position");
        CHECK_EQUAL_TEXT(temp[i], value, "Wrong value read");
    }
    state = circ_buffer_read_value(&buffer, &value);
    CHECK_EQUAL_TEXT(1, state, "Wrong state returned");
    CHECK_EQUAL_TEXT((BUFFER_SIZE / 2) - 1, buffer.tail, "Tail at wrong position");
    CHECK_EQUAL_TEXT(temp[(BUFFER_SIZE / 2) - 1], value, "Wrong value read");
}
