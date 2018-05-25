#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../lib/libclimate.h"

float temperature;
float dew_point;
float rel_humidity;
float longitude;

int main(int argc, char const *argv[])
{
    temperature = temperature_generator();
    float pressure = gen_pressure(temperature);
    rel_humidity = relative_humidity_calculator();
    dew_point = dew_point_calculator(rel_humidity);
    float humidity =  gen_humidity(dew_point, rel_humidity);
    longitude = longitude_generator();
    float windspeed = gen_wind_speed(longitude);

    printf("Pressure value is: %f  \n", pressure);
    printf("Humidity value is: %f \n", humidity);
    printf("Wind speed value is: %f \n", windspeed);
    return 0;
}
