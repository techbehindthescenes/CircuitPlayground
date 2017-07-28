// Demo of Adafruit Circuit Playground 
// Author: Tech Behind The Scenes (www.techbehindthescenes.com)
// License: MIT License (https://opensource.org/licenses/MIT)

// Uses the CircuitPlayground library to easily use the full functionality of the board

#include <Adafruit_CircuitPlayground.h>

/********************************************************************
 * Musical Notes via https://www.arduino.cc/en/Tutorial/ToneMelody by Tom Igoe *
 ********************************************************************/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define RED 0
#define ORANGE 21
#define YELLOW 42
#define GREEN 85
#define BLUEGREEN 127
#define BLUE 170
#define PURPLE 220
#define PINK 235

#define CAP_THRESHOLD 200

int defaultColor; 
int chosenColor; //color - selected by capacitance touch buttons
boolean colorSelected; //true if user selects a color
float X, Y, Z; //from accelerometer
int soundSensorValue;
int lightValue; 
int mode; //0, 1, 2, 3
boolean slideSwitch; //true = left side; false = right side

//====================================================================
// State and Control Helpers
//====================================================================
void reset() {
  CircuitPlayground.setBrightness(30);
  CircuitPlayground.clearPixels();
}

void setMode() {
  slideSwitch = CircuitPlayground.slideSwitch(); //left
  if (slideSwitch) {
    if(CircuitPlayground.leftButton()) {
        mode = 0;
    } else if (CircuitPlayground.rightButton()) {   
        mode = 1;
    } else {
        mode = 100;
        reset();
    }
  } else {
    if (CircuitPlayground.leftButton()) {
      mode = 2;
    } else if (CircuitPlayground.rightButton()) {   
      mode = 3;
    } else {
      mode = 200;
      reset();
    }
  }
}

boolean stateHasChanged() {
  boolean slidePositionChange = (slideSwitch != CircuitPlayground.slideSwitch());
  //if (slidePositionChange)
  //{
  //  Serial.println("Slide switch changed positions");
  //}
  return ( slidePositionChange || CircuitPlayground.rightButton() || CircuitPlayground.leftButton());
}

//====================================================================
// LED Neopixel Color Helpers
//====================================================================
void setColor() {
  if (CircuitPlayground.readCap(3) > CAP_THRESHOLD) {
    chosenColor = RED;
  } else if (CircuitPlayground.readCap(2) > CAP_THRESHOLD) {
    chosenColor = ORANGE;
  } else if (CircuitPlayground.readCap(0) > CAP_THRESHOLD) {
    chosenColor = YELLOW;
  } else if (CircuitPlayground.readCap(1) > CAP_THRESHOLD) {
    chosenColor = GREEN;
  } else if (CircuitPlayground.readCap(12) > CAP_THRESHOLD) {
    chosenColor = BLUEGREEN;
  } else if (CircuitPlayground.readCap(6) > CAP_THRESHOLD) {
    chosenColor = BLUE;
  } else if (CircuitPlayground.readCap(9) > CAP_THRESHOLD) {
    chosenColor = PURPLE;
  } else if (CircuitPlayground.readCap(10) > CAP_THRESHOLD) {
    chosenColor = PINK;
  } else if (!colorSelected) {
    chosenColor = defaultColor;
  }
  if (!colorSelected and chosenColor != defaultColor) {
     Serial.print("Chosen color is: ");
     Serial.println(chosenColor);
     colorSelected = true;
  }
}

void setColorAllPixels(int color, int brightness) {
  CircuitPlayground.clearPixels();
  for (int i=0; i<10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, CircuitPlayground.colorWheel(color));
  }
  CircuitPlayground.setBrightness(brightness);
  CircuitPlayground.strip.show();
}

//====================================================================
// Music Helpers
//====================================================================
void playNote(int note, int duration) {
  // to calculate the note duration, take one second divided by the note type.
  //e.g. quarter note = 800 / 4, eighth note = 800/8, etc.
  int defaultSpeed = 800;
  int noteDuration = defaultSpeed / duration;
  CircuitPlayground.playTone(note, noteDuration);
  // to distinguish the notes, set a minimum time between them.
  //   the note's duration + 30% seems to work well:
  int pauseBetweenNotes = noteDuration * 1.30;
  delay(pauseBetweenNotes);
}

void playNoteWithLight(int note, int duration, int led) {
  CircuitPlayground.setPixelColor(led, CircuitPlayground.colorWheel(25 * led));
  playNote(note, duration);
  CircuitPlayground.clearPixels();
}

void playNoteWithLights(int note, int duration) {
  switch(note) {
    case NOTE_C4 :  
      playNoteWithLight(NOTE_C4, duration, 1);
      break;
    case NOTE_D4 : 
      playNoteWithLight(NOTE_D4, duration, 2);
      break;
    case NOTE_E4 :  
      playNoteWithLight(NOTE_E4, duration, 3);
      break;
    case NOTE_FS4 :
      playNoteWithLight(NOTE_FS4, duration, 4);
      break;
    case NOTE_F4 :
      playNoteWithLight(NOTE_F4, duration, 4);
    break;
    case NOTE_G4 :
      playNoteWithLight(NOTE_G4, duration, 5);
      break;
    case NOTE_A4 :
      playNoteWithLight(NOTE_A4, duration, 6);
      break;
    case NOTE_B4 :
      playNoteWithLight(NOTE_B4, duration,7);
      break;
    case NOTE_C5 :
      playNoteWithLight(NOTE_C5, duration, 8);
      break;
    case NOTE_D5 :
      playNoteWithLight(NOTE_D5, duration, 9);
      break; 
    case 0 :
      playNote(0, duration);
      break;
  }
}

void playHappyBirthday() {
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_D4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_FS4, 2);
      
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_D4, 4);
      playNoteWithLights(NOTE_A4, 4);
      playNoteWithLights(NOTE_G4, 2);
      
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_D5, 4);
      playNoteWithLights(NOTE_B4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_FS4, 4);
      playNoteWithLights(NOTE_E4, 4);
      
      playNoteWithLights(NOTE_C5, 8);
      playNoteWithLights(NOTE_C5, 8);
      playNoteWithLights(NOTE_B4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_A4, 4);
      playNoteWithLights(NOTE_G4, 2);
}

void playABCs() {
      playNoteWithLights(NOTE_C4, 4);
      playNoteWithLights(NOTE_C4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_A4, 4);
      playNoteWithLights(NOTE_A4, 4);
      playNoteWithLights(NOTE_G4, 2);

      playNoteWithLights(NOTE_F4, 4);
      playNoteWithLights(NOTE_F4, 4);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_D4, 8);
      playNoteWithLights(NOTE_C4, 2);

      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_F4, 2);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_D4, 2);

      playNoteWithLights(NOTE_G4, 8);
      playNoteWithLights(NOTE_G4, 8);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_F4, 2);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_D4, 2);

      playNoteWithLights(NOTE_C4, 4);
      playNoteWithLights(NOTE_C4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_G4, 4);
      playNoteWithLights(NOTE_A4, 4);
      playNoteWithLights(NOTE_A4, 4);
      playNoteWithLights(NOTE_G4, 2);
      
      playNoteWithLights(NOTE_F4, 4);
      playNoteWithLights(NOTE_F4, 4);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_E4, 4);
      playNoteWithLights(NOTE_D4, 4);
      playNoteWithLights(NOTE_D4, 4);
      playNoteWithLights(NOTE_C4, 2);
}

//====================================================================
// Setup and Main Loop
//====================================================================

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();  // initialize the CP library
  defaultColor = PURPLE;
  colorSelected = false;
}

void loop() {
  setMode();
  setColor();
  //Serial.print("Mode is: ");
  //Serial.println(mode);
  if (stateHasChanged()) {
    //Serial.println("State has changed.");
    reset();
  }

  //===================================
  // Mode 0 = play happy birthday once per left button click
  // Left slide, left button click
  //===================================
  if (mode == 0) {    
    playHappyBirthday();
    //playABCs();
    mode = 500; //hack: need to press the button again for it to play again
  }

  //===================================
  // Mode 1: Night light - gets brighter as it detects less light
  // Left slide, right button click
  //===================================
  while (mode == 1 && !stateHasChanged()) {
    lightValue = CircuitPlayground.lightSensor();
    //Serial.print("light value is: ");
    //Serial.println(lightValue);
    if (lightValue < 20) {  //dark
      setColorAllPixels(chosenColor, 80);
    } else {
      setColorAllPixels(chosenColor, 10);
    }
  }

  //===================================
  // Mode 2: Show led light at "bottom" of a gyrating circuit playground; uses accelerometer
  // Right slide, left button click

  //===================================
  while (mode == 2 && !stateHasChanged()) {    
      X = CircuitPlayground.motionX();
      Y = CircuitPlayground.motionY();
      Z = CircuitPlayground.motionZ();

//      Serial.print("X: ");
//      Serial.print(X);
//      Serial.print("  Y: ");
//      Serial.print(Y);
//      Serial.print("  Z: ");
//      Serial.println(Z);

//  NEWER Mode 2 code - with all neopixels
 
     int pin = -1;
     int scale = 3.5; 
    // scale = scale * (abs(X) / 9.5); 
     int YScale = int(abs(Y)/scale);
     
      if (X > 0 && Y > 0) {
         pin = 5 ;
         pin += YScale;
      } 

      if (X < 0 && Y > 0) {
         pin = 9;
         pin -= YScale;
      } 


      if (X > 0 && Y < 0) {
         pin = 4;
         pin -= YScale;
      }

     if (X < 0 && Y < 0) {
         pin = 0;
         pin += YScale;
      }

        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor(pin, CircuitPlayground.colorWheel(chosenColor));

//  OLDER Mode 2 code with only 4 pixels

//      if (X > 0 && Y > 0) {
//        CircuitPlayground.clearPixels();
//        CircuitPlayground.setPixelColor(6, CircuitPlayground.colorWheel(chosenColor));
//      } 
//
//      if (X > 0 && Y < 0) {
//        CircuitPlayground.clearPixels();
//        CircuitPlayground.setPixelColor(3, CircuitPlayground.colorWheel(chosenColor));
//      }
//      if (X < 0 && Y > 0) {
//        CircuitPlayground.clearPixels();
//        CircuitPlayground.setPixelColor(8, CircuitPlayground.colorWheel(chosenColor));
//      }
//      if (X < 0 && Y < 0) {
//        CircuitPlayground.clearPixels();
//        CircuitPlayground.setPixelColor(1, CircuitPlayground.colorWheel(chosenColor));
//      }
    }

    //===================================
    // Mode 3: Show sound levels using colors
    // Right slide, right button click
    //===================================
    while (mode == 3 && !stateHasChanged()) {
        soundSensorValue = CircuitPlayground.soundSensor();
        //Serial.print("Sound Sensor Value is: ");
        //Serial.println(soundSensorValue);
        if (soundSensorValue <= 350) {
          CircuitPlayground.clearPixels();
        } else if (soundSensorValue <= 390) {
            setColorAllPixels(GREEN, 40);
        } else if (soundSensorValue <= 450) {
            setColorAllPixels(YELLOW, 80);
        } else {
            setColorAllPixels(RED, 120);
        }
    }   
}
