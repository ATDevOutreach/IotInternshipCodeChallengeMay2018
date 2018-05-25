#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include <math.h>
#include "../lib/libclimate.h"

#ifdef _WIN32
#define vsnprintf _vsnprintf
#endif /* _WIN32 */

static float temperature = 0.00;
static float dew_point = 0.00;
static float rel_humidity = 0.00;
static float longitude = 0.00;
longitude = longitude_generator();
temperature = temperature_generator();
rel_humidity = relative_humidity_calculator();
dew_point = dew_point_calculator(rel_humidity);


static void test_random_seed_function(void **state)
{
    (void) state;
    float u_bound = 15.00;
    float l_bound = 1.00;
    srand(time(0));
    float range_values = (u_bound - l_bound) + 1;
    float value1 = l_bound + (float)(range_values * rand() / (RAND_MAX + 1.00));
    float value2 = l_bound + (float)(range_values * rand() / (RAND_MAX + 1.00));
    assert_false(value2 == value1);
}

static void test_pressure_data(void **state)
{
    (void) state;
    float temp = temperature_generator();
    float pressure = 0.00;
    pressure = gen_pressure(temp);
    assert_true(pressure > 97000);
}

static void test_humidity_data(void **state)
{
    (void) state;
    float humidity_val_1, humidity_val_2 = 0.00;
    humidity_val_1 = gen_humidity(dew_point, temperature);
    temperature = temperature_generator();
    rel_humidity = relative_humidity_calculator();
    dew_point = dew_point_calculator(rel_humidity); 
    humidity_val_2 = gen_humidity(dew_point, temperature);
    assert_true((humidity_val_2 != humidity_val_1));
}

static void test_wind_speed_data(void **state)
{
    (void) state;
    float wind_speed = 0.00;
    wind_speed = gen_wind_speed(longitude);
    assert_true(wind_speed < 20.00);
}

int main(void)
{   
    const struct CMUnitTest testcases[] ={
        cmoka_unit_test(test_humidity_data),
        cmoka_unit_test(test_wind_speed_data),
        cmoka_unit_test(test_pressure_data),
        cmoka_unit_test(test_random_seed_function),
    };

    return cmoka_run_group_tests(testcases, NULL, NULL);
}
