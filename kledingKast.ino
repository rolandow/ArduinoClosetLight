#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>
#include "closetLight.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 9

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

const int SENSOR_LEFT_PIN = 2;
const int SENSOR_RIGHT_PIN = 3;

#define ARRAY_SIZE 2
#define TIMEOUT 5*60*1000

closetLight lights[ARRAY_SIZE] = {
  closetLight(SENSOR_LEFT_PIN, 0, 60, &strip, TIMEOUT),
  closetLight(SENSOR_RIGHT_PIN, 60, 120, &strip, TIMEOUT),
};


void setup() {
  
  Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  for (int i=0; i < ARRAY_SIZE; i++) 
  {
    lights[i].init();
  }
}



void loop() {
  for (int i=0; i < ARRAY_SIZE; i++) 
  {
    lights[i].process();
  }
}

//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
//  }
//}
//
//
//void colorWipe(uint32_t c, uint8_t wait, uint8_t start, uint8_t number) {
////  for(uint16_t i=0; i<strip.numPixels(); i++) {
//  uint8_t stopAt = start+number;
//  for(uint16_t i=start; i<stopAt; i++) {
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
//  }
//}

