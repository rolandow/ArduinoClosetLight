#include "closetLight.h"

closetLight::closetLight(int s, int f, int l, Adafruit_NeoPixel *str, unsigned long t) {
  sensorPin = s;
  firstLed = f;
  lastLed = l;
  strip = str;
  currentState = 0;
  timeOut = t;
}

closetLight::~closetLight() {/*nothing to destruct*/}

void closetLight::init() {
    pinMode(sensorPin, INPUT_PULLUP);
    debouncer.attach(sensorPin);
    debouncer.interval(30);

    Serial.print("P ");
    Serial.print(sensorPin);
    Serial.print(" ");
    Serial.print(firstLed);
    Serial.print(" ");
    Serial.print(lastLed);
    Serial.print(" ");
    Serial.print(timeOut);
    
    Serial.println();

    initLed();
}

void closetLight::initLed() {
  for (int i = firstLed; i < lastLed; i++) {
    strip->setPixelColor(i, strip->Color(255,0,0));
    strip->show();
  }
  delay(1000);
  for (int i = firstLed; i < lastLed; i++) {
    strip->setPixelColor(i, strip->Color(0,0,0));
    strip->show();
  }
}

void closetLight::process() {
  debouncer.update();
  bool state = debouncer.read();
  if (currentState != state) {
    currentState = state;

    if (currentState == true) {
      color = strip->Color(0,0,255);
      onAt = millis();
    }
    else {
      color = strip->Color(0,0,0);
    }

    ledPos = firstLed;
    
    Serial.print("St ");
    Serial.print(sensorPin);
    Serial.print(" ");
    Serial.print(state);
    Serial.println();
    
  }

  if (ledPos < lastLed) {
    strip->setPixelColor(ledPos, color);
    strip->show();
    ledPos++;
  }

  if (currentState == true && (onAt > 0) && (millis()-onAt >= timeOut)) 
  {
    onAt = 0;
    Serial.println("Auto off");
    color = strip->Color(0,0,0);
    ledPos = firstLed;
  }
}

