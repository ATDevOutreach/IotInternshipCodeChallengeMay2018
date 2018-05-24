import math
from decimal import Decimal
import random
import pprint as pprint


class Libclimate:


    def gen_humidity (self, dew_point, temperature):
        humidity = 100.00 * ((611 * math.exp(5423 * ((1 / 273)) - (1 / (dew_point + 273)))) / (611 * math.exp(5423 * ((1 / 273)-(1 / temperature)))))
        print('humidity' , str(humidity))
        return(humidity)
       
          
    def gen_pressure(self, temperature):
         pressure = 101325 * math.exp(((0.00 - 9.81) * 0.0289644 * (200))/(8.31444598 * (temperature + 273)))
         print('pressure' , str(pressure))
         return (pressure)
         
         

    def gen_wind_speed(self, longitude):
        wind_speed = ((0.0001) * longitude)
        print('wind_speed' , str(wind_speed))
        return ( wind_speed)
       
        
        
    def dew_point_calculator(self, relative_humidity):
        dew_point = (100.00 - relative_humidity) / 5
        print('dew_point' , str(dew_point))
        return (dew_point)
        
        

    def relative_humidity_calculator(self):
        upper_hum = 75.00
        lower_hum = 55.00
        relative_humidity = random.uniform(lower_hum, upper_hum)
        print ('relative_humidity' , str(relative_humidity))
        return(relative_humidity)
        

    def temperature_generator(self):
        upper_temp = 42.00
        lower_temp = 15.00
        temperature = random.uniform(lower_temp, upper_temp)
        print('temperature' , str(temperature))
        return (temperature)
        
        
    def longitude_generator(self):
        upper_l = 98.00
        lower_l = 102.05
        longitude = random.uniform(lower_l, upper_l)
        print('longitude' , str(longitude))
        return (longitude)


