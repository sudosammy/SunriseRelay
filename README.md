# README

A simple relay that will switch on at sunrise and off at sunset. Each day it will fetch the latest sunrise and sunset data based on your longitude and latitude.

The microcontroller is consumption conscious and minimises unnecessary power draw (using the ESP8266 Light Sleep function). 

Since the days are much longer than nights in Australia, the relay is configured to latch during the night as this is when it draws most power.

## Power Usage

| Relay State | Board Consumption | 240V Watt Meter |
| ----------- | ----------- | ----------- |
| Unlatched (i.e. Daytime) | Min: <10mA Max: 150mA | Avg: 0.2W |
| Latched (i.e. Nighttime) | Min: 70mA Max: 150mA | Avg: 1.5W |

## Future improvements

1. Currently using an LM7805 which should be swapped to a more efficient voltage regulator
2. The board contains two unnecessary LEDs which should be removed.
3. Utilising the ESP8266 Deep Sleep function but this requires challenging soldering of the board to enable.