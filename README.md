# Arduino-Attiny-LED-Controller

This opensource product is a PCB where you can use 5 digital pins to control a LED strip. 
This came from the fact that we use Siemens 24V PLC's on school, but they are unable to do such things.
With a little bit tweeking you could change this voltage to your's sirired voltage


EasyEda PCB link
https://easyeda.com/jellewietsma/ATtinyLEDcontroller


# Connections and control
I1+2+3 can be conbined to create diffrent colors, for example if there all HIGH it would be WHITE

I1 - Turns RED LEDs on
I2 - Turns GREEN LEDs on
I3 - Turns BLUE LEDs on

I4 - Animation blink (like a turn signal)
I5 - Animation Flash
I4+I5 - Animation Flash other way
!I4&!I5 - just steady on