# Obstacle-avoider
Given an inputs (destination point in a 2d plane), the car will move to the position specified while avoiding abstacle on its way 
An Intel Galileo was used for this project
# What do you need
* a car (something with at least two motors, one for each direction)
* a 5 pins joystick (V+,X,Y,Z,GND)
* a push button
* an lcd screen
* a circuit to drive the motor from the micro
* a servo
* a position sensor place on top of the servo

# How to build it
* servo to pin 5
* position sensor to pin A2
* X of joystick to pin A0
* Y of joystick to pin A1
* Z of joystick to pin 12
* left motor to pin 10
* right motor to pin 11
* lcd: Rs=2, E=4, D4=6, D5=7, D6=8, D7=9

# How to run it
* inputs: coordinates are entered using the joystick, each direction increment the position by 10cm in that direction.
if the push button is pressed then the direction increases by 2cm in the direction previously chose by the joystick.
* validate input: press the z button on the joystick to validate the coordinates.
* Car should automatically try to go to the disired position.
