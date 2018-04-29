#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "bsp.h"
    #include "adc_driver.h"
}

#include "test_rcc_mock.h"
#include "test_gpio_mock.h"

TEST_GROUP(AdcDriverTestGroup){
    void setup(void){
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    }

    void teardown(void){
        mock().clear();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

/* For a passing test the folloing has to happen:
 * Enable peripheral clock to timer and GPIOs
 * Call Timer Init with proper settings
 * Call PWM Init twice with proper settings
 * Call GPIO Init with proper settings
 */
TEST(AdcDriverTestGroup, PWMInitProperly){
    FAIL("This should fail.");
}
