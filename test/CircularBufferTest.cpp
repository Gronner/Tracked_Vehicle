#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
}

TEST_GROUP(CircularBufferTestGroup){

    void setup(void){
    }

    void teardown(void){
    }
};

TEST(CircularBufferTestGroup, CircularBufferInitProperly){
    FAIL("This should fail");
}
