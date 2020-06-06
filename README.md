# AirControl

##Concept
The idea is to automatically control flow of air to the heat pump with
servomotor attached to pipe. The current valve condition will depend on 
comparision of outside and inside temperature and be visible at LCD together 
with current temperatures. It should also be possible to control the valve 
manually, if needed. All diagnostic information will be available at local web 
page.
##Components needed: 
* microcontroller
* two temperature sensors
* Wi-fi module
* servomotor 
* amplifier circuit to ensure 0-10V output
* LCD screen, buttons
* power circuit (Power supply + at least 1 step-down converter)

##Expected problems:
* creating www page
* providing power to:
  * servo(24V DC), 
  * amplifier circuit (10-12V),
  * controller(3-5V) - depends on controller 