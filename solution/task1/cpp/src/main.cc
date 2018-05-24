#include <iostream>
#include <cstdlib>
#include "../lib/libclimate.hpp"

using namespace std;
Libclimate climate;
int main()
{
    float humidity;
    float wind_speed;
    float pressure;
    float longitude;
    float temp; // Temperature
    float dewp; // Dew Point
    float r_h; // Relative Humidity
    temp = climate.temperature_generator();
    r_h = climate.relative_humidity_calculator();
    dewp = climate.dew_point_calculator(r_h);
    longitude = climate.longitude_generator();
    humidity = climate.gen_humidity(dewp, temp);
    longitude = climate.longitude_generator();
    pressure = climate.gen_pressure(temp);
    wind_speed = climate.gen_wind_speed(longitude);
    cout << "Humidity: " <<  humidity  << "%" << endl;
    cout << "Pressure: " << pressure << "milliBars" << endl;
    cout << "Wind speed: " << wind_speed << "m/s" << endl;
    return 0;
}