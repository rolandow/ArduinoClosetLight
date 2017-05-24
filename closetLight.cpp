#include "closetLight.h"

closetLight::closetLight(int s, int f, int l, Adafruit_NeoPixel *str, unsigned long t) {
  sensorPin = s;
  firstLed = f;
  lastLed = l;
  strip = str;
  currentState = 0;
  timeOut = t;
  onColor = str->Color(0,0,255);
}

closetLight::closetLight(int s, int f, int l, Adafruit_NeoPixel *str, unsigned long t, uint32_t c) {
  sensorPin = s;
  firstLed = f;
  lastLed = l;
  strip = str;
  currentState = 0;
  timeOut = t;
  onColor = c;
}


closetLight::~closetLight() {/*nothing to destruct*/}

void closetLight::init() {
    pinMode(sensorPin, INPUT_PULLUP);
    debouncer.attach(sensorPin);
    debouncer.interval(30);

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
      color = onColor;
      onAt = millis();
    }
    else {
      color = strip->Color(0,0,0);
    }

    ledPos = firstLed;
  }

  if (ledPos < lastLed) {
    strip->setPixelColor(ledPos, color);
    strip->show();
    ledPos++;
  }

  if (currentState == true && (onAt > 0) && (millis()-onAt >= timeOut)) 
  {
    onAt = 0;
    color = strip->Color(0,0,0);
    ledPos = firstLed;
  }
}

