# AirControl

## Concept
The idea is to automatically control flow of air to the heat pump with
servomotor attached to pipe. The current valve condition will depend on 
comparision of outside and inside temperature and be visible at LCD together 
with current temperatures. It should also be possible to control the valve 
manually, if needed. All diagnostic information will be available at local web 
page.
## Components needed: 
* microcontroller - ESP32 development board
* two temperature sensors - DS18B20
* ~~Wi-fi module~~ - board has it embedded
* servomotor - Alnor DM-ML-06-24 or Alnor DM-ML-06-230
* amplifier circuit to ensure 0-10V output - based on LM358
* LCD screen, buttons - LCD1602 based on HD44780 with I2C converting module
* power circuit (Power supply + at least 1 step-down converter) - depends mostly
on servomotor

## Expected problems:
* creating www page
* providing power to:
  * servo(24V DC / 230V AC), 
  * amplifier circuit (10-12V),
  * controller(3-5V) - depends on controller 