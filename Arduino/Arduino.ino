/*
  Program written by JelleWho for a standaone chip that takes 5 digtal inputs and convert them to a FastLed controlled led strip
*/
//#define FASTLED_INTERNAL
#include <FastLED.h>                                        //Include the FastLED library to control the LEDs in a simple fashion
//static const (read only) █ <variable type> https://www.arduino.cc/reference/en/#variables █ <Pin ║ Digital, Analog, pWn ║ Input, Output║ _name>
static const byte PAO_LED = 0;                                      //Where the <LED strip> is connected to
static const byte PDI_Red = 1;                                      //turn RED LEDs on
static const byte PDI_Green = 2;                                    //turn Green LEDs on
static const byte PDI_Blue = 3;                                     //turn Blue LEDs on
static const byte PDI_Blink = 4;                                    //animation = Blinking like a turn signal
static const byte PDI_Flash = 5;                                    //animation = Just flash all together (else (&!^) just steady color)
//Just some configurable things
static const int TotalLEDs = 10;
const static byte DelayanimationBlink = 50;                         //Delay in ms for the animation (minimum time)
const static byte DelayanimationFlash = 200;                        //^^
//Just some numbers we need to transfer around..
CRGB LEDs[TotalLEDs];                                               //This is an array of LEDs. One item for each LED in your strip.
void setup() {                                                      //This code runs once on start-up
  pinMode(PAO_LED,    OUTPUT);                                      //Sometimes the Arduino needs to know what pins are OUTPUT and what are INPUT,
  pinMode(PDI_Red,    INPUT);                                       // since it could get confused and create an error. So it's set manual here
  pinMode(PDI_Green,  INPUT);                                       //^^
  pinMode(PDI_Blue,   INPUT);                                       //^^
  pinMode(PDI_Blink,  INPUT);                                       //^^
  pinMode(PDI_Flash,  INPUT);                                       //^^
  FastLED.addLeds<WS2812B, PAO_LED, GRB>(LEDs, TotalLEDs);          //Set the LED type and such
  FastLED.setBrightness(80);                                        //Scale brightness
  fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));                 //Set the whole LED strip to be off (Black)
  FastLED.show();                                                   //Update
}
void loop() {                                                       //Keep looping the next code
  static bool FlashStateOn = false;
  byte Red = 0;                                                     //Create a place to store value of RED in, default = off
  byte Green = 0;                                                   //^^
  byte Blue = 0;                                                    //^^
  if (digitalRead(PDI_Red) == HIGH)                                 //If the Red LEDs need to be on
    Red = 255;                                                      //Max them out so they will be on
  if (digitalRead(PDI_Green) == HIGH)                               //^^
    Green = 255;                                                    //^^
  if (digitalRead(PDI_Blue) == HIGH)                                //^^^^
    Blue = 255;                                                     //^^^^
  if (digitalRead(PDI_Blink) == HIGH) {                             //If the LEDs need to show an animation
    EVERY_N_MILLISECONDS(DelayanimationBlink) {                     //Do every 'DelayanimationBlink' ms
      static byte Counter;                                          //Create a counter (storing the amounts of LEDs on)
      static byte Offset = 0;                                       //Start LED's number to start the blink from
      static byte Length = TotalLEDs;                               //Total length of LEDs to blink
      static byte AlwaysOn = 1;                                     //Howmuch LEDs should always be on
      fill_solid(&(LEDs[Offset]), Length,   CRGB(0, 0, 0));         //Turn LEDs off
      fill_solid(&(LEDs[Offset]), AlwaysOn, CRGB(255, 128, 0));     //Set some LEDs to be always on
      fill_solid(&(LEDs[Offset]), Counter,  CRGB(255, 128, 0));     //Set the counter amount of LEDs on (this will increase)
      Counter++;                                                    //This will make the front LED section length bigger
      if (Counter > Length)                                         //If we are at max length
        Counter = 0;                                                //Reset counter
    }
  } else if (digitalRead(PDI_Flash) == HIGH) {                      //If the LEDs need a flashing animation
    EVERY_N_MILLISECONDS(DelayanimationFlash) {                     //Do every 'DelayanimationBlink' ms
      FlashStateOn = !FlashStateOn;                                 //Toggle the state
      if (FlashStateOn)                                             //If the flash needs to be on
        fill_solid(&(LEDs[0]), TotalLEDs, CRGB(Red, Green, Blue));  //Set the whole LED strip to be the right color
      else
        fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));           //Set the whole LED strip to be off (Black)
    }
  } else                                                            //else we just need to show the colors
    fill_solid(&(LEDs[0]), TotalLEDs, CRGB(Red, Green, Blue));      //Set the whole LED strip to be off (Black)
  FastLED.show();                                                   //Update the LEDs
}
//this is the end, hope you had fun
