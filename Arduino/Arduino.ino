/*
  Program written by JelleWho for a standalone chip that takes 5 digital inputs and convert them to a FastLed controlled led strip
*/

//Just some configurable things
const static int TotalLEDs = 60;                                    //The total amounts of LEDs in the strip
const static byte DelayAnimationBlink = 50;                         //Delay in ms for an update in the animation (technically the minimum time)
const static byte DelayAnimationFlash = 250;                        //^^
const static byte DelayAnimationMove = 20;                          //^^
const static byte Brightness = 255;                                 //The brightness of the LEDs (higher is brightener)
const static byte MoveAmount = 2;                                   //Quantity of the sections in the move animation
const static byte MoveLength = 5;                                   //Length of the sections in the move animation


//static const (read only) █ <variable type> https://www.arduino.cc/reference/en/#variables █ <Pin ║ Digital, Analog, pWn ║ Input, Output║ _name>
static const byte PAO_LED = 0;                                      //Where the <LED strip> is connected to
static const byte PDI_Red = 1;                                      //Turn RED LEDs on
static const byte PDI_Green = 2;                                    //Turn Green LEDs on
static const byte PDI_Blue = 3;                                     //Turn Blue LEDs on
static const byte PDI_Blink = 4;                                    //animation = Blinking like a turn signal
static const byte PDI_Flash = 0;                                    //animation = Just flash all together (else (&!^) just steady color)  (THIS IS THE ANALOG PIN NUMBER)
#include <FastLED.h>                                                //Include the FastLED library to control the LEDs in a simple fashion
CRGB LEDs[TotalLEDs];                                               //This is an array of LEDs. One item for each LED in your strip.
void setup() {                                                      //This code runs once on start-up
  pinMode(PAO_LED,    OUTPUT);                                      //Sometimes the Arduino needs to know what pins are OUTPUT and what are INPUT,
  pinMode(PDI_Red,    INPUT);                                       //Since it could get confused and create an error. So it's set manual here
  pinMode(PDI_Green,  INPUT);                                       //^^
  pinMode(PDI_Blue,   INPUT);                                       //^^
  pinMode(PDI_Blink,  INPUT);                                       //^^
  pinMode(PDI_Flash,  INPUT);                                       //^^
  FastLED.addLeds<WS2812B, PAO_LED, GRB>(LEDs, TotalLEDs);          //Set the LED type and such
  FastLED.setBrightness(Brightness);                                //Scale brightness
  fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));                 //Set the whole LED strip to be off (Black)
  FastLED.show();                                                   //Update
}
void loop() {                                                       //Keep looping the next code
  static byte Mode = 0;                                             //What animation Mode we are in, each time this changes, we need to remove the old LED state
  /*
    Mode 1 = blink (Triggered by I4)
    Mode 2 = Flash (I5)
    Mode 3 = Move (I4+I5)
    Mode 4 = Steady (!I4 & !I5)
  */
  bool UpdateLEDs = false;                                          //This needs to be flagged TRUE if we change something, and the LEDs need an update
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
    if (analogRead(PDI_Flash) >= 900) {                             //If the LEDs need to show both animations, (so show the third animation)
      if (Mode != 3) {                                              //If we just moved into this Mode (and are not yet in the correct state)
        Mode = 3;                                                   //Set this Mode
        fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));           //Set the whole LED strip to be off (Black)
        UpdateLEDs = true;                                          //Update
      }
      static int CounterMove = 0;                                   //Create a counter (To store the position in)
      int OffsetMove = TotalLEDs / MoveAmount;                      //Calculation for calculating offset from first Position
      int poss[MoveAmount];                                         //Array for saving the positions of the sections
      if (CounterMove >= TotalLEDs) {                               //Will check if the main position is bigger than the total
        CounterMove = 0;                                            //If that is the case than it will reset it to 0
      } else {
        EVERY_N_MILLISECONDS(DelayAnimationMove) {                  //Do every 'DelayAnimationBlink' ms
          CounterMove++;                                            //It will just set it to 0
          fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));
          UpdateLEDs = true;                                        //Update
        }
      }
      for (int i = 0; i < MoveAmount; i++) {                        //Beginning of the loop which will send each position and length
        poss[i] = CounterMove + (OffsetMove * i);                   //This will calculate each position by adding the offset times the position number to the first position
        int posX;                                                   //This is the variable which will be used for sending position start
        if (poss[i] >= TotalLEDs) {                                 //To see if the position is to bigger than the total amount
          posX = poss[i] - TotalLEDs;                               //Subtract the total amount of LEDs from the position number
        } else                                                      //Otherwise it will just use the position data without modifying it
          posX = poss[i];                                           //Just use the position number
        if (posX <= (TotalLEDs - MoveLength)) {                     //If the whole section ends before the total amount is reached it will just us the normal way of setting the LEDs
          fill_solid(&(LEDs[posX]), MoveLength, CRGB(Red, Green, Blue)); //With the standard fill solid command from FastLED, LEDs[posX] PosX stands for beginning position, MoveLength will stand for the size of the sections and the last one is the color
        } else if ((posX >= (TotalLEDs - MoveLength)) && (posX <= TotalLEDs)) {//This will check if the thing is beyond the total amount of LEDs
          int calc1 = (TotalLEDs - (posX + MoveLength)) * -1;       //Calculates the amount of LEDs which need to be set from the beginning
          int calc2 = MoveLength - calc1;                           //Calculates the amount of LEDs which need to be set at the last so the total will be reached but won’t be bigger than the total
          fill_solid(&(LEDs[posX]), calc2, CRGB(Red, Green, Blue)); //Fills the LEDs at the end of the strip
          fill_solid(&(LEDs[0])   , calc1, CRGB(Red, Green, Blue)); //Fills the LEDs at the beginning of the strip
        }
      }
    } else {
      if (Mode != 1) {                                              //If we just moved into this Mode (and are not yet in the correct state)
        Mode = 1;                                                   //Set this Mode
        fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));           //Set the whole LED strip to be off (Black)
        UpdateLEDs = true;                                          //Update
      }
      EVERY_N_MILLISECONDS(DelayAnimationBlink) {                   //Do every 'DelayAnimationBlink' ms
        UpdateLEDs = true;                                          //Update
        static byte CounterBlink = 0;                               //Create a counter (storing the amounts of LEDs on)
        static byte LengthBlink = TotalLEDs;                        //Total length of LEDs to blink
        static byte AlwaysOn = 1;                                   //How much LEDs should always be on
        fill_solid(&(LEDs[0]), LengthBlink,  CRGB(0, 0, 0));        //Turn LEDs off
        fill_solid(&(LEDs[0]), AlwaysOn,     CRGB(Red, Green, Blue));//Set some LEDs to be always on
        fill_solid(&(LEDs[0]), CounterBlink, CRGB(Red, Green, Blue));//Set the counter amount of LEDs on (this will increase)
        CounterBlink++;                                             //This will make the blink 1 longer each time
        if (CounterBlink > LengthBlink)                             //If we are at max length
          CounterBlink = 0;                                         //Reset counter
      }
    }
  } else if (analogRead(PDI_Flash) >= 900) {                        //If the LEDs need a flashing animation
    if (Mode != 2) {                                                //If we just moved into this Mode (and are not yet in the correct state)
      Mode = 2;                                                     //Set this Mode
      fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));             //Set the whole LED strip to be off (Black)
      UpdateLEDs = true;                                            //Update
    }
    static bool FlashStateOn = false;                               //Create a bool to store the flash state in
    EVERY_N_MILLISECONDS(DelayAnimationFlash) {                     //Do every 'DelayAnimationBlink' ms
      FlashStateOn = !FlashStateOn;                                 //Toggle the state
      if (FlashStateOn)                                             //If the flash needs to be on
        fill_solid(&(LEDs[0]), TotalLEDs, CRGB(Red, Green, Blue));  //Set the whole LED strip to be the right color
      else
        fill_solid(&(LEDs[0]), TotalLEDs, CRGB(0, 0, 0));           //Set the whole LED strip to be off (Black)
      UpdateLEDs = true;                                            //Update
    }
  } else {                                                          //else we just need to show the colors
    Mode = 4;                                                       //Set the Mode
    fill_solid(&(LEDs[0]), TotalLEDs, CRGB(Red, Green, Blue));      //Set the whole LED strip to be off (Black)
    UpdateLEDs = true;                                              //Update
  }
  if (UpdateLEDs)                                                   //If the LEDs need an update (something has changed in the array, so we need push the update though)
    FastLED.show();                                                 //Update the LEDs
}
//this is the end, hope you had fun
