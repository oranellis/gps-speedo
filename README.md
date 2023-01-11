# GPS Speedo
## Overview
An esp8266/8285 based, high-refresh GPS speedometer, to be used for acceleration and top speed testing in various applications. Designed to be a cheaper alternative to more expensive timing systems, it makes use of any ublox M8 based GPS module, though has currently only been tested with a BN220. To display speed, an SSD1306 based 128x64 OLED display is used. Code from iForce2D on youtube was used to interpret the UBX protocol messages from the GPS, and the U8G2 library is used to drive the display.
## Current Features
- 10Hz GPS updates
- Interpolation to display at 60Hz, adding roughly 100ms delay
- Acceleration timing, with adjustable start and end speed (currently limited to max of 255, to be improved)
- Basic menu system, based on 3 button navigation
- Ability to change units, currently km/h, mph and m/s

## Planned Features
- Add distance timing (quarter mile etc.)
- Add accelerometer for g-force sensing
- Add Kalman Filter to increase GPS accuracy
