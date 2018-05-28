#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/libclimate.h"

#ifdef _WIN32
#define vsnprintf _vsnprintf
#endif /* _WIN32 */

float temperature;
float dew_point;
float rel_humidity;

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
    temperature = temperature_generator();
    float pressure;
    pressure = gen_pressure(temperature);
    assert_true(pressure > 97000);
}

static void test_humidity_data(void **state)
{
    (void) state;
    float humidity_val_1, humidity_val_2;
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
    float wind_speed;
    float longitude = longitude_generator();
    wind_speed = gen_wind_speed(longitude);
    assert_true(wind_speed < 20.00);
}

int main(void)
{   
    const struct CMUnitTest testcases[] = {
        cmocka_unit_test(test_humidity_data),
        cmocka_unit_test(test_wind_speed_data),
        cmocka_unit_test(test_pressure_data),
        cmocka_unit_test(test_random_seed_function),
    };

    return cmocka_run_group_tests(testcases, NULL, NULL);
}
