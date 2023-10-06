#include <unity.h>
#include "PI_controller.h"

void setUp(void)
{
    // This function is run before EACH test is executed.
}

void tearDown(void)
{
    // This function is run after EACH test is executed.
}

void test_initialization_and_reset(void)
{
    PI_controller controller(1.0, 1.0, 0.1);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, controller.get_integral());
    controller.update(60, 0.0); // Just to change the integral value
    TEST_ASSERT_TRUE(controller.get_integral() > 0.0);
    controller.reset_integral();
    TEST_ASSERT_EQUAL_DOUBLE(0.0, controller.get_integral());
}

void test_integral_control(void)
{
    PI_controller controller(1.0, 1.0, 0.1);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, controller.get_integral());
    controller.update(60, 0.0);
    TEST_ASSERT_TRUE(controller.get_integral() > 0.0);
}


void test_setting_and_getting_parameters(void)
{
    PI_controller controller(1.0, 1.0, 0.1);
    controller.set_Kp(2.0);
    controller.set_Ti(2.0);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, controller.get_Kp());
    TEST_ASSERT_EQUAL_DOUBLE(2.0, controller.get_Ti());
}

void test_integral_windup(void) {
    PI_controller controller(1.0, 100, 1);

    // Force saturation for multiple iterations
    for (int i = 0; i < 1000; i++) {
        controller.update(60.0, 0.0);
    }

    // Check that the integral hasn't winded up excessively
    double integral_value = controller.get_integral();
    TEST_ASSERT_EQUAL(0, controller.get_integral());
}

void run_tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_initialization_and_reset);
    RUN_TEST(test_setting_and_getting_parameters);
    RUN_TEST(test_integral_windup);
    RUN_TEST(test_integral_control);
    UNITY_END();
}

int main()
{
    run_tests();
    return 0;
}
