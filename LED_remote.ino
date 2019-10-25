#include<stdio.h>

//  Include following libraries to access IR sensor
#include <IRremote.h>
#include <IRremoteInt.h>

//  Include following libraries to control the LEDS without delay()
#include <Curve.h>
#include <LEDFader.h>

const int RED_LED_PIN = 6;
const int GRN_LED_PIN = 9;
const int BLU_LED_PIN = 10;

#define LED_NUM 3

LEDFader leds[LED_NUM] = {
  LEDFader(6),
  LEDFader(9),
  LEDFader(10)
};

const int fadeDelay = 10; // Time for delay
const int smoothDelay = 100; // Time for delay
const int reallySlowhDelay = 500; // Time for delay

int IR_RECV_PIN = 2;          //  The digital pin that the signal pin of the sensor is connected to
IRrecv receiver(IR_RECV_PIN);  //  Create a new receiver object that would decode signals to key codes
decode_results results;     //  A varuable that would be used by receiver to put the key code into

// -------- Functions ----------


const int LED_OFF = 0;
const int LED_ON = 255;


struct color_t {
  unsigned char R, G, B;
};

//--- Colors ---
struct color_t whiteColor       = {255, 255, 255};
struct color_t blackColor       = {0, 0, 0};
struct color_t redColor         = {255, 0, 0};
struct color_t greenColor       = {0, 255, 0};
struct color_t blueColor        = {0, 0, 255};
struct color_t orangeColor      = {255, 176, 41};
struct color_t limeColor        = {55, 255, 33};
struct color_t purpleColor      = {100, 60, 255};
struct color_t lightOrangeColor = {255, 255, 50};
struct color_t skyBlueColor     = {66, 191, 245};
struct color_t pinkColor        = {252, 3, 252};
struct color_t yellowColor      = {255, 255, 0};
struct color_t darkBlueColor    = {29, 27, 81};
struct color_t lightPinkColor   = {255, 71, 252};


int color(int value) {
  return abs(LED_OFF - value);
}

void changeColor(struct color_t newColor) {
  analogWrite(RED_LED_PIN, color(newColor.R));
  analogWrite(GRN_LED_PIN, color(newColor.G));
  analogWrite(BLU_LED_PIN, color(newColor.B));
}

void strobeColors() {
  // Update all LEDs and start new fades if any are done
  for (byte i = 0; i < LED_NUM; i++) {
    LEDFader *led = &leds[i];
    led->update();

    // This LED is not fading, start a new fade
    if (led->is_fading() == false) {
      int duration = random(1000, 3000); // between 1 - 3 seconds

      // Fade Up
      if (led->get_value() == 0) {
        byte intensity = random(100, 255);
        led->fade(intensity, duration);
      }
      // Fade Down
      else {
        led->fade(0, duration);
      }
    }
  }
}

void fadeColors(int timeInterval) {
  LEDFader *led1 = &leds[0];
  LEDFader *led2 = &leds[1];
  LEDFader *led3 = &leds[2];

  led1->update();
  led2->update();
  led3->update();

  bool finishedFading = (led1->is_fading() == false) && (led2->is_fading() == false) && (led3->is_fading() == false);
  int duration = timeInterval * 1000;

  if (finishedFading == false) {
    return;
  }
  
  if (led1->is_fading() == false) {
    if (led1->get_value() == 0) {
      led2->fade(255, duration);
      led3->fade(0, duration);
    } else {
      led2->fade(0, duration);
      led3->fade(255, duration);
    }
  }

  if (led3->is_fading() == false) {
    if (led3->get_value() == 0) {
      led1->fade(255, duration);
      led2->fade(0, duration);
    } else {
      led1->fade(0, duration);
      led2->fade(255, duration);
    }
  }

  if (led2->is_fading() == false) {
    if (led2->get_value() == 0) {
      led1->fade(255, duration);
      led3->fade(0, duration);
    } else {
      led1->fade(0, duration);
      led3->fade(255, duration);
    }
  }
}

void flashColors() {
  // Update all LEDs and start new fades if any are done
  for (byte i = 0; i < LED_NUM; i++) {
    LEDFader *led = &leds[i];
    led->update();

    // This LED is not fading, start a new fade
    if (led->is_fading() == false) {
      int duration = 5000; // between 1 - 3 seconds

      // Fade Up
      if (led->get_value() == 0) {
        byte intensity = random(100, 255);
        led->fade(intensity, duration);
      }
      // Fade Down
      else {
        led->fade(0, duration);
      }
    }
  }
}


enum program {off = 0, on = 1, solidColor = 2, fadeing = 3, strobe = 4, flash = 5, smooth = 6};
enum program currentProgram = on;

void buttonPressed(decode_results buttonValue) {
  if (buttonValue.value == 0xFF629D) {
    Serial.println("BTN_OFF");
    currentProgram = off;
    changeColor(blackColor);
  }
  if (buttonValue.value == 0xFFA25D) {
    Serial.println("BTN_ON");
    currentProgram = on;
    changeColor(whiteColor);
  }
  if (buttonValue.value == 0xFFE21D) {
    Serial.println("BTN_FLASH");
    changeColor(blackColor);
    currentProgram = flash;
  }
  if (buttonValue.value == 0xFF22DD) {
    Serial.println("BTN_STROBE");
    changeColor(blackColor);
    currentProgram = strobe;
  }
  if (buttonValue.value == 0xFF02FD) {
    Serial.println("BTN_FADE");
    changeColor(blackColor);
    currentProgram = fadeing;
  }
  if (buttonValue.value == 0xFFC23D) {
    Serial.println("BTN_SMOOTH");
    changeColor(blackColor);
    currentProgram = smooth;
  }
  if (buttonValue.value == 0xFFE01F) {
    Serial.println("BTN_BR_UP");
  }
  if (buttonValue.value == 0xFFA857) {
    Serial.println("BTN_BR_DOWN");
  }

  //--- Colors ---
  if (buttonValue.value == 0xFF906F) {
    Serial.println("BTN_WHITE");
    currentProgram = solidColor;
    changeColor(whiteColor);
  }
  if (buttonValue.value == 0xFF6897) {
    Serial.println("BTN_RED");
    currentProgram = solidColor;
    changeColor(redColor);
  }
  if (buttonValue.value == 0xFF9867) {
    Serial.println("BTN_GREEN");
    currentProgram = solidColor;
    changeColor(greenColor);
  }
  if (buttonValue.value == 0xFFB04F) {
    Serial.println("BTN_BLUE");
    currentProgram = solidColor;
    changeColor(blueColor);
  }
  if (buttonValue.value == 0xFF30CF) {
    Serial.println("BTN_ORANGE");
    currentProgram = solidColor;
    changeColor(orangeColor);
  }
  if (buttonValue.value == 0xFF18E7) {
    Serial.println("BTN_LIME");
    currentProgram = solidColor;
    changeColor(limeColor);
  }
  if (buttonValue.value == 0xFF7A85) {
    Serial.println("BTN_PURPLE");
    currentProgram = solidColor;
    changeColor(purpleColor);
  }
  if (buttonValue.value == 0xFF10EF) {
    Serial.println("BTN_LIGHT_ORANGE");
    currentProgram = solidColor;
    changeColor(lightOrangeColor);
  }
  if (buttonValue.value == 0xFF38C7) {
    Serial.println("BTN_SKY_BLUE");
    currentProgram = solidColor;
    changeColor(skyBlueColor);
  }
  if (buttonValue.value == 0xFF5AA5) {
    Serial.println("BTN_PINK");
    currentProgram = solidColor;
    changeColor(pinkColor);
  }
  if (buttonValue.value == 0xFF42BD) {
    Serial.println("BTN_YELLOW");
    currentProgram = solidColor;
    changeColor(yellowColor);
  }
  if (buttonValue.value == 0xFF4AB5) {
    Serial.println("BTN_DARK_BLUE");
    currentProgram = solidColor;
    changeColor(darkBlueColor);
  }
  if (buttonValue.value == 0xFF52AD) {
    Serial.println("BTN_LIGHT_PINK");
    currentProgram = solidColor;
    changeColor(lightPinkColor);
  }
}


// --------- SETUP ----------
void setup() {
  //  Setup serial port to send key codes to computer
  Serial.begin(9600);

  //  Setup the pins connected to LEDs as output pin
  pinMode(BLU_LED_PIN, OUTPUT);
  pinMode(GRN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  //  Setup the pin for the interrupt
  //  pinMode(IR_RECV_PIN, INPUT_PULLUP);
  //  attachInterrupt(digitalPinToInterrupt(IR_RECV_PIN), blink, CHANGE);

  changeColor(whiteColor);

  receiver.enableIRIn();    //  Enable receiver so that it would start processing infrared signals
}

// --------- MAIN LOOP ----------
void loop() {
  if (receiver.decode(&results)) {            //  Decode the button code and put it in "results" variable
    buttonPressed(results);
    Serial.println(results.value, HEX);
    receiver.resume();                        //  Continue listening for new signals
  }

  switch (currentProgram) {
    case off: break;
    case on: break;
    case solidColor: break;
    case fadeing: fadeColors(8); break;
    case smooth: fadeColors(25); break;
    case strobe: strobeColors(); break;
    case flash: flashColors(); break;
    default: printf("not defined"); break;
  }
}

void blink() {
  printf("INTERRUPT");
}
