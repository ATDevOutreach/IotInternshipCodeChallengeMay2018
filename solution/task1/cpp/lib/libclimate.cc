#include <math.h>
#include <cstdlib>
#include <ctime>

#include "libclimate.hpp"

float Libclimate::gen_humidity(float dew_point, float temperature)
{
    float humidity = 100.00 * ((611 * expf(5423 * ((1 / 273)) - (1 / (dew_point + 273)))) / (611 * expf(5423 * ((1 / 273)-(1 / temperature)))));
    return humidity;
}

float Libclimate::gen_pressure(float temperature)
{
    float pressure = 101325 * expf(((0.00 - 9.81) * 0.0289644 * (200))/(8.31444598 * (temperature + 273)));
    return pressure;
}

float Libclimate::gen_wind_speed(float longitude)
{
    float wind_speed = ((0.0001) * longitude);
    return wind_speed;
}

float Libclimate::dew_point_calculator(float relative_humidity)
{
    float dew_point = (100.00 - relative_humidity) / 5;
    return dew_point;
}

float Libclimate::relative_humidity_calculator(void)
{
    srand((unsigned)time(0));
    float upper_hum = 75.00;
    float lower_hum = 55.00;
    float humidity_range = (upper_hum - lower_hum) + 1;
    float relative_humidity = lower_hum + (float)(humidity_range * rand() / (RAND_MAX + 1.00));
    return relative_humidity;
}

float Libclimate::temperature_generator(void)
{
    srand((unsigned)time(0));
    float upper_temp = 42.00;
    float lower_temp = 15.00;
    float temp_range = (upper_temp - lower_temp) + 1;
    float temperature = lower_temp + (float)(temp_range * rand() / (RAND_MAX + 1.00));
    return temperature;
}

float Libclimate::longitude_generator(void)
{
    srand((unsigned)time(0));
    float upper_l = 102.05;
    float lower_l = 98.00;
    float l_range = (upper_l - lower_l) + 1;
    float longitude = lower_l + (float)(l_range * rand() / (RAND_MAX + 1.00));
    return longitude;
}