#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include <stdio.h>
    #include "stm32f4xx_conf.h"
    #include "bsp.h"
    #include "i2c_driver.h"
}


TEST_GROUP(I2CDriverTestGroup){

    void setup(void){
    }

    void teardown(void){
    }
};

TEST(I2CDriverTestGroup, FailTest){
    FAIL("This should fail");
}
