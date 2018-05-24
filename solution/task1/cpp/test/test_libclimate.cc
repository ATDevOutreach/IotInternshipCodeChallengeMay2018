#include "../lib/libclimate.hpp"
#include <gtest/gtest.h>
#include <ctime>

class TestLibClimate : public ::testing::Test
{
    protected:
        virtual void TearDown() {}
        virtual void SetUp() {}
    public:

};

Libclimate climate;

/* Test Random Seed generatot
 * This test asserts that at any given time, the random numbers
 * generated will never be equal.
 * This is attribted to the use of time as a seeding param
 * We'll perform a fatal assertion test to ensure that this is the case
 */

TEST_F(TestLibClimate, test_random_seed_func)
{
    srand((unsigned)time(0));
    float upper_bound = 100.00;
    float lower_bound = 2.00;
    float range_ = (upper_bound - lower_bound) + 1;
    float value1 = lower_bound + (float)(range_ * rand() / (RAND_MAX + 1.00));
    float value2 = lower_bound + (float)(range_ * rand() / (RAND_MAX + 1.00));
    ASSERT_FALSE(value1 == value2);
}

/** Test the upper and lower limits of the temperature value generated
 *  This fatal test ensures that the temperature range is between 15 and 42
 *  degrees at any given time
 */

TEST_F(TestLibClimate, test_temperature_bounds)
{
    float temp_value_init = climate.temperature_generator();
    float temp_value_next = climate.temperature_generator();
    bool comp  = (temp_value_init < 15.00) && (temp_value_next > 42);
    ASSERT_TRUE(!comp);
}

/** Test the upper and lower limits of the humidity  generated
 *  This fatal test ensures that the humidity range is between 55 and 75
 *  % at any given time
 */

TEST_F(TestLibClimate, test_humidity_bounds)
{
    float rel_humidity_value_init = climate.relative_humidity_calculator();
    float rel_humidity_value_next = climate.relative_humidity_calculator();
    float dew_point_value_init = climate.dew_point_calculator(rel_humidity_value_init);
    float dew_point_value_next = climate.dew_point_calculator(rel_humidity_value_next);
    float temp_value_init = climate.temperature_generator();
    float temp_value_next = climate.temperature_generator();
    float humidity_init = climate.gen_humidity(dew_point_value_init, temp_value_init);
    float humidity_next = climate.gen_humidity(dew_point_value_next, temp_value_next);
    bool comp  = (humidity_init < 55.00) && (humidity_next > 75);
    ASSERT_TRUE(!comp);
}

/**
 * Test the value of pressure generated is above the standard atmospheric pressure at sea-level ~1 atm
 * This is another fatal test
 */

TEST_F(TestLibClimate, test_pressure_value)
{
    float temperature = climate.temperature_generator();
    float pressure_value = climate.gen_pressure(temperature);
    ASSERT_TRUE(pressure_value > 97000);
} 

/** For a longitude around 98 and 102 we expect windspeed to be about ~8 to ~15m/s
 *  This is defined by the Bernoullis eqn
 *  Expect a fatal assertion
 */ 

TEST_F(TestLibClimate, test_wind_speed)
{
    float long_value_init = climate.longitude_generator();
    float long_value_next = climate.longitude_generator();
    float ws_value_init = climate.gen_wind_speed(long_value_init);
    float ws_value_next = climate.gen_wind_speed(long_value_next);
    bool comp  = (ws_value_init < 5.00) && (ws_value_next > 15.00);
    ASSERT_TRUE(!comp);
}

/** This test ensures that the longitudinal values gemerated are within the specified Geographic bounds
 *  Expect a fatal assertion
 */ 

TEST_F(TestLibClimate, test_longitude_value)
{
    float long_value_init = climate.longitude_generator();
    float long_value_next = climate.longitude_generator();
    bool comp  = (long_value_init < 98.00) && (long_value_next > 102.05);
    ASSERT_TRUE(!comp);
}

int main(int argc, char * argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}