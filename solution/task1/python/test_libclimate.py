import unittest
import math 
from libclimate import Libclimate

climate = Libclimate()


class TestClimate(unittest.TestCase):
   
    def test_gen_humidity(self):
       #test humidity generation
       dew_point = 11
       temperature = 30
       calculated_humidity =  100.00 * ((611 * math.exp(5423 * ((1 / 273)) - (1 / (dew_point + 273)))) / (611 * math.exp(5423 * ((1 / 273)-(1 / temperature)))))
       humidity = climate.gen_humidity(dew_point, temperature)
       self.assertTrue(humidity == calculated_humidity)

    def test_gen_pressure(self):
        #testing pressure generation
        temperature = 30
        pressure = climate.gen_pressure(temperature)
        self.assertTrue(pressure >= 97000)
        
        
    def test_wind_speed(self):
        #testing wind speed
        longitude = 100
        calculated_windspeed = ((0.0001) * longitude)
        wind_speed = climate.gen_wind_speed(longitude)
        self.assertTrue(wind_speed==calculated_windspeed)

    def test_dew_point_calculator(self):
        #test dew point
        relative_humidity = 45
        calulated_dew_point = (100-relative_humidity)/5
        dew_point = climate.dew_point_calculator(relative_humidity)
        self.assertTrue(dew_point==calulated_dew_point)

    
    def test_relative_humidity_calculator(self):
        #test relative humidity
        relative_humidity = climate.relative_humidity_calculator()
        self.assertTrue( 55 <= relative_humidity <= 75)

    def test_temperature_generator(self):
        #test temperature generation
        temperature = climate.temperature_generator()
        self.assertTrue(15 <= temperature <= 42)
    
    def test_longitude_generator(self):
        #test longitude generation
        longitude = climate.longitude_generator()
        self.assertTrue(  98.00 <= longitude <= 102.05 )

if __name__ == '__main__':
    unittest.main()

