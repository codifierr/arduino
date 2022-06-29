# Home Automation

Automatic Water controller for Arduino based on the Ultrasonic sensor.

[]: # Language: c++
[]: # Path: ultra_sonic/ultra_sonic.ino
[]: # Compare this snippet from LICENSE:
[]: # MIT License
[]: #
[]: # Path: ultra_sonic/ultra_sonic.ino

## About the Project

This project make use of the [Ultrasonic Sensor](https://www.arduino.cc/en/Reference/UltrasonicSensor) to control the water level. It is a simple project that can be used to control the water level of a water tank. When the water level is low based on predefined value, the water level sensor will send a signal to the Arduino. The Arduino will then turn on the water pump to make the water level higher.

## Features

AutoStart on board startup.
AutoStop on when water level is reached.
Dryrun protection in case of water level is not increasing with a defined time to prevent the pump from running forever.
Retry mechanism in case pump turned by Dryrun protection.

## Future enhancements

Scheduling of the water level sensor to run at a certain time.
