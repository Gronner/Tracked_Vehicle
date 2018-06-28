#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include <stdio.h>
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
    CHECK_EQUAL_TEXT(0, 
                     memcmp(buffer.memory, temp, sizeof(buffer.memory)),
                     "Memory content not cleared properly");
    CHECK_EQUAL_TEXT(0, buffer.head, "Head at wrong position"); // Head should be at zero
    // Tail should be at the empty memory position
    CHECK_EQUAL_TEXT(0, buffer.tail, "Tail at the wrong position"); 
}

TEST(CircularBufferTestGroup, CircularBufferResetProperly){
    for(uint8_t i = 0; i < BUFFER_SIZE + 1; i++){
        buffer.memory[i] = i + 1;
    }
    buffer.head = 12;
    buffer.tail = 8; 

    circ_buffer_reset(&buffer);

    uint16_t temp[BUFFER_SIZE + 1] = {}; // In C++ this sets all values to zero!
    CHECK_EQUAL(0, memcmp(buffer.memory, temp, sizeof(buffer.memory)));
    CHECK_EQUAL(0, buffer.head); // Head should be at zero
    CHECK_EQUAL(0, buffer.tail); // Tail should be at buffer end
}

TEST(CircularBufferTestGroup, CircularBufferAddOne) {
    circ_buffer_reset(&buffer);

    uint16_t new_number = 1;

    circ_buffer_add_value(&buffer, new_number);

    CHECK_EQUAL_TEXT(1, buffer.memory[0], "Wrong value in buffer");
    CHECK_EQUAL_TEXT(1, buffer.head, "Head at wrong position");
    CHECK_EQUAL_TEXT(0, buffer.tail, "Tail at wrong position");
}

TEST(CircularBufferTestGroup, CircularBufferAddTwo) {
    circ_buffer_reset(&buffer);

    uint16_t new_number1 = 1;
    uint16_t new_number2 = 2;

    circ_buffer_add_value(&buffer, new_number1);
    circ_buffer_add_value(&buffer, new_number2);

    CHECK_EQUAL_TEXT(new_number1, buffer.memory[0], "Wrong value 1 in buffer");
    CHECK_EQUAL_TEXT(new_number2, buffer.memory[1], "Wrong value 2 in buffer");
    CHECK_EQUAL_TEXT(2, buffer.head, "Head at wrong position");
    CHECK_EQUAL_TEXT(0, buffer.tail, "Tail at wrong position");
}
    
TEST(CircularBufferTestGroup, CircularBufferAddFill) {
    circ_buffer_reset(&buffer);

    uint16_t temp[BUFFER_SIZE + 1] = {};
    
    for(uint8_t i = 0; i < BUFFER_SIZE; i++) {
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }

    for(uint8_t i = 0; i < BUFFER_SIZE; i++) {
        CHECK_EQUAL_TEXT(temp[i], buffer.memory[i], "Wrong value in buffer");
    }
    CHECK_EQUAL_TEXT(BUFFER_SIZE, buffer.head, "Head at wrong position");
    CHECK_EQUAL_TEXT(0, buffer.tail, "Tail at wrong position");
}

TEST(CircularBufferTestGroup, CircularBufferAddOverfill) {
    circ_buffer_reset(&buffer);
    
    uint16_t temp[BUFFER_SIZE + 2] = {};

    for(uint8_t i = 0; i < BUFFER_SIZE + 1; i++) {
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }

    for(uint8_t i = 0; i < BUFFER_SIZE + 1; i++) {
        CHECK_EQUAL_TEXT(temp[i], buffer.memory[i], "Wrong value in buffer");
    }
    CHECK_EQUAL_TEXT(0, buffer.head, "Head at wrong value");
    CHECK_EQUAL_TEXT(1, buffer.tail, "Tail at wrong position");
}

TEST(CircularBufferTestGroup, CircularBufferReadOne) {
    circ_buffer_reset(&buffer);

    uint16_t temp = 12;
    uint16_t target = 0;
    uint8_t status = 9;

    circ_buffer_add_value(&buffer, temp);

    status = circ_buffer_read_value(&buffer, &target);
    CHECK_EQUAL_TEXT(temp, target, "Wrong value returned");
    CHECK_EQUAL_TEXT(0, status, "Wrong status returned");
    CHECK_EQUAL_TEXT(1, buffer.head, "Head at wrong position");
    CHECK_EQUAL_TEXT(1, buffer.tail, "Tail at wrong position");
}

TEST(CircularBufferTestGroup, CircularBufferReadTwo) {
    circ_buffer_reset(&buffer);

    uint16_t temp[] = {12, 13};
    uint16_t target = 0;

    circ_buffer_add_value(&buffer, temp[0]);
    circ_buffer_add_value(&buffer, temp[1]);

    circ_buffer_read_value(&buffer, &target);
    CHECK_EQUAL_TEXT(temp[0], target, "Wrong value returned");
    circ_buffer_read_value(&buffer, &target);
    CHECK_EQUAL_TEXT(temp[1], target, "Wrong value returned");
    CHECK_EQUAL_TEXT(2, buffer.head, "Head at wrong position");
    CHECK_EQUAL_TEXT(2, buffer.tail, "Tail at wrong position");
}

TEST(CircularBufferTestGroup, CircularBufferReadEmpty) {
    circ_buffer_reset(&buffer);
     
    uint16_t target = 0;
    uint8_t status = 0;

    status = circ_buffer_read_value(&buffer, &target);
    
    CHECK_EQUAL_TEXT(1, status, "Wrong status returned");
    CHECK_EQUAL_TEXT(0, buffer.head, "Head at wrong position");
    CHECK_EQUAL_TEXT(0, buffer.tail, "Tail at wrong position");
}

TEST(CircularBufferTestGroup, CircularBufferReadOverfill){
    circ_buffer_reset(&buffer);
    
    uint16_t temp[BUFFER_SIZE + 2] = {};

    for(uint8_t i = 0; i < BUFFER_SIZE + 1; i++) {
        temp[i] = i + 1;
        circ_buffer_add_value(&buffer, i + 1);
    }

    uint8_t status = 9;
    uint16_t target = 0;
    for(uint8_t i = 0; i < BUFFER_SIZE; i++) {
        status = circ_buffer_read_value(&buffer, &target);
        CHECK_EQUAL_TEXT(0, status, "Wrong status returned");
        CHECK_EQUAL_TEXT(temp[i + 1], target, "Wrong value returned");
    }
    status = circ_buffer_read_value(&buffer, &target);
    CHECK_EQUAL_TEXT(1, status, "Wrong status returned");
    CHECK_EQUAL_TEXT(0, buffer.head, "Head at wrong position");
    CHECK_EQUAL_TEXT(0, buffer.tail, "Tail at wrong position");
}
