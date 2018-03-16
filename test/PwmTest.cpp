#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "bsp.h"
    #include "pwd_driver.h"
}

TEST_GROUP(PwdDriverTestGroup){
    void setup(void){
    }

    void teardown(void){
    }
};
