#include <math.h>
#include <Arduino.h>
#include "libclimate.hpp"

float Libclimate::gen_humidity(float dew_point, float temperature)
{
    float humidity = 100.00 * ((611 * exp(5423 * ((1 / 273)) - (1 / (dew_point + 273)))) / (611 * exp(5423 * ((1 / 273)-(1 / temperature)))));
    return humidity;
}

float Libclimate::gen_pressure(float temperature)
{
    float pressure = 101325 * exp(((0.00 - 9.81) * 0.0289644 * (200))/(8.31444598 * (temperature + 273)));
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
    randomSeed(analogRead(0));
    float upper_hum = 7500;
    float lower_hum = 5500;
    float relative_humidity = (random(lower_hum, upper_hum)) / 100;
    return relative_humidity;
}

float Libclimate::temperature_generator(void)
{
    randomSeed(analogRead(0));
    float upper_temp = 42.00;
    float lower_temp = 15.00;
    float temperature = (random(lower_temp, upper_temp)) / 100;
    return temperature;
}

float Libclimate::longitude_generator(void)
{
    randomSeed(analogRead(0));
    float upper_l = 102.05;
    float lower_l = 98.00;
    float longitude = (random(lower_l, upper_l)) / 100;
    return longitude;
}