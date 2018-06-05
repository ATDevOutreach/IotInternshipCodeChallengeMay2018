#include "libclimate.hpp"

Libclimate libclimate;

void setup(void);
void loop(void);

float _temp=0.00;
float _relh=0.00;
float _longitude=0.00;
float _dewpoint=0.00;
float pressure_=0.00;
float windspeed_=0.00;
float humidity_=0.00;

void setup()
{
    
    while(!Serial);
    Serial.begin(9600);
    Serial.println(F("Setting up for Lib-Climate"));
    uint8_t i = 0;
    while(i <= 10){
        Serial.print(F("="));
    i++;
    }
    Serial.println();
}

void loop()
{
    _temp = libclimate.temperature_generator();
    _relh = libclimate.relative_humidity_calculator();
    _longitude = libclimate.longitude_generator();
    _dewpoint = libclimate.dew_point_calculator(_relh);
    humidity_ = libclimate.gen_humidity(_dewpoint,_temp);
    pressure_ = libclimate.gen_pressure(_temp);
    windspeed_ = libclimate.gen_wind_speed(_longitude);
    Serial.print(F("Humidity data:"));
    Serial.println(humidity_);
    delay(100);
    Serial.print(F("Pressure data:"));
    Serial.println(pressure_);
    delay(100);
    Serial.println(F("Wind-speed data:"));
    Serial.println(windspeed_);
    delay(3000);
}