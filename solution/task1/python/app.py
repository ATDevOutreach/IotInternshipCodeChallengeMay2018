import math
from decimal import Decimal
from random import randint
import libclimate

climate = libclimate.Libclimate()


relative_humidity = climate.relative_humidity_calculator()
dew_point = climate.dew_point_calculator(relative_humidity)
temperature = climate.temperature_generator()
hum = climate.gen_humidity(dew_point, temperature)
longitude = climate.longitude_generator()

climate.gen_humidity(dew_point, temperature)
climate.gen_pressure(temperature)
climate.dew_point_calculator(relative_humidity)
climate.relative_humidity_calculator()
climate.temperature_generator()
climate.longitude_generator()
climate.gen_wind_speed(longitude)