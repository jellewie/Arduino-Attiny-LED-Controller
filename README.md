# Arduino-Attiny-LED-Controller

This opensource product is a PCB where you can use 5 digital pins to control a LED strip. 
This came from the fact that we use Siemens 24V PLC's on school, but they are unable to do such things.
With a little bit tweaking you could change this voltage to yours desired voltage


EasyEda PCB link
https://easyeda.com/jellewietsma/ATtinyLEDcontroller


# Connections and control
I1+2+3 can be combined to create different colors, for example if there all HIGH it would be WHITE

I1 - Turns RED LEDs on
I2 - Turns GREEN LEDs on
I3 - Turns BLUE LEDs on

I4 - Animation blink (Like a turn signal)
I5 - Animation Flash (Just flashing everything)
I4+I5 - Animation Move (Moves LEDs around)
!I4&!I5 - just steady on

# Manual
The manual can be find in either as a word document or as PDF, there called "Manual"