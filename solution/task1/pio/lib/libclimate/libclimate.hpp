/**
 * This is a simple library that generates Pressure, Humidity and Wind-speed data
 * Given Barometric pressure in milliBars as : 101325 * exp(((0.00 - 9.81) * 0.0289644 * (200))/(8.31444598 * (temp + 273)))
 * Given Winspeed for a given longitude as : (0.0001) * long
 * Given Humdity as : 100.00 * ((611 * exp(5423 * ((1 / 273)) - (1 / (dp + 273)))) / (611 * exp(5423 * ((1 / 273)-(1 / temp)))))
 */

#if !defined(LIBCLIMATE_HPP)
#define LIBCLIMATE_HPP

class Libclimate
{
private:

public:
    Libclimate() {}
    float gen_pressure(float temperature);
    float gen_wind_speed(float longitude);
    float gen_humidity(float dew_point, float temperature);
    float temperature_generator();
    float dew_point_calculator(float relative_humidity);
    float relative_humidity_calculator();
    float longitude_generator();
};

#endif // LIBCLIMATE_HPP
