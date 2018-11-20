# ardio
Arduino firmware that makes it a customizable USB I/O peripheral

To use the library, make sure you have all the required tools installed on your system.
you should be able to build from the Arduino java IDE.

This package assumes Arduino 1.8.7 is installed in /opt/arduino/arduino-1.8.7

in the ardio directory, type make

To upload the image to the arduino, type make upload

The "unittest" module in the examples directory showshow to use this system.

The "ranger" uses the ultrasonic module to measure

The "readtemp" uses the DS18B20 module to measure temperature.

To build the DS18B20 module you need "OneWire-2.3.4" and the "Arduino-Temperature-Control-Library"
installed along side ardio. 

https://github.com/milesburton/Arduino-Temperature-Control-Library

(There are better docs on the way)
