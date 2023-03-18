# README

![PLants](/plants.jpg)

A simple relay that will switch on at sunrise and off at sunset. Each day it will fetch the latest sunrise and sunset data based on your longitude and latitude.

The microcontroller is consumption conscious and minimises unnecessary power draw (using the ESP8266 Light Sleep function). 

Since the days are much longer than nights in Australia, the relay is configured to latch during the night as this is when it draws most power.

## Power Usage

|Voltage Reg | Relay State | Board Consumption | 240V Watt Meter |
| ------ | ----------- | ----------- | ----------- |
| [LM7805](https://www.altronics.com.au/p/z0505-7805-5v-1-amp-to-220-fixed-voltage-regulator/) | Unlatched (i.e. Daytime) | Min: <10mA Max: 150mA | Avg: 0.2W |
| [LM7805](https://www.altronics.com.au/p/z0505-7805-5v-1-amp-to-220-fixed-voltage-regulator/) | Latched (i.e. Nighttime) | Min: 70mA Max: 150mA | Avg: 1.5W |
| [D4012SA](https://www.aliexpress.com/item/1005002168374063.html) | Unlatched (i.e. Daytime) | | Avg: 0.2W |
| [D4012SA](https://www.aliexpress.com/item/1005002168374063.html) | Latched (i.e. Nighttime) | | Avg: 0.5W |

## Future improvements

1. Two unnecessary LEDs (one on the ESP01 which is always on, and one on the relay board which is on when latched) could be removed.
2. Utilising the ESP8266 Deep Sleep function but this requires [challenging soldering](https://www.tech-spy.co.uk/2019/04/enable-deep-sleep-esp-01/) of the board to enable.

![The board](/board.jpg)