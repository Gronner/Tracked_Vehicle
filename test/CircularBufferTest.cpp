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

    uint16_t temp[BUFFER_SIZE] = {}; // In C++ this sets all values to zero!
    // See if all values are inizialized to zero
    CHECK_EQUAL(0, memcmp(buffer.memory, temp, sizeof(buffer.memory)));
    CHECK_EQUAL(0, buffer.head); // Head should be at zero
    CHECK_EQUAL(0, buffer.tail); // Tail should be at zero
}

TEST(CircularBufferTestGroup, CircularBufferResetProperly){
    for(uint8_t i = 0; i < BUFFER_SIZE; i++){
        buffer.memory[i] = i;
    }
    buffer.head = 12;
    buffer.tail = 8; 

    circ_buffer_reset(&buffer);

    uint16_t temp[BUFFER_SIZE] = {}; // In C++ this sets all values to zero!
    CHECK_EQUAL(0, memcmp(buffer.memory, temp, sizeof(buffer.memory)));
    CHECK_EQUAL(0, buffer.head); // Head should be at zero
    CHECK_EQUAL(0, buffer.tail); // Tail should be at zero
}

