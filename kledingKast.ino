#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 9


class doorSensor {
    int pin;
    bool currentPosition = false;
    
    unsigned long positionChangedAt;
    Bounce debouncer = Bounce();

  public:
  doorSensor(int sensorPin) {
    Serial.print("Initialising with pin ");
    Serial.println(sensorPin);
    
    pin = sensorPin;

    pinMode(pin, INPUT_PULLUP);
    debouncer.attach(pin);
    debouncer.interval(30);
    currentPosition = false;
  }

  void Update() {
    debouncer.update();
    bool newPosition = debouncer.read();
    if (newPosition != currentPosition) {
      Serial.print("Sensor ");
      Serial.print(pin);
      Serial.print(" changed from ");
      Serial.print(currentPosition);
      Serial.print(" to ");
      Serial.println(newPosition);
      
      currentPosition = newPosition;
      positionChangedAt = millis();
    }
  }

  bool isOpen() {
    return currentPosition;
  }

  bool isClosed() {
    return !currentPosition;
  }

  unsigned long statusChangedAt() {
    return positionChangedAt;
  }

  int getPin() {
    return pin;
  }
};


class ledStrip {
  bool currentState = false;
  unsigned long stateChangedAt;
  String stripName;

  uint16_t startPixel=0;
  uint16_t endPixel=120;
  uint32_t color;

  Adafruit_NeoPixel * strip;  

  public:
  ledStrip(String myName, Adafruit_NeoPixel * s, uint16_t sPixel, uint16_t ePixel) {
    stripName = myName;
    strip = s;
    startPixel = sPixel;
    endPixel = ePixel;
  }
  
  void turnOn() {
    changeState(true);
  }

  void turnOff() {
    changeState(false);
  }
  
  public:
  bool isOn() {
    return currentState;
  }

  public:
  bool isOff() {
    return currentState;
  }

  unsigned long getStateChangedAt()
  {
    return stateChangedAt;
  }

  private:
  void changeState(bool newState) {
    if (currentState != newState) {
      Serial.print("Turning strip ");
      Serial.print(stripName);
      Serial.print(" to state ");
      Serial.println(newState);

      if (newState)
        color = strip->Color(255,255,255);
      else
        color = strip->Color(0,0,0);

      for(uint16_t i=startPixel; i<endPixel; i++) {
        strip->setPixelColor(i, color);
        strip->show();
      }


      
      currentState = newState;
      stateChangedAt = millis();
    }
  }

};

class closetLight {
  doorSensor * sensor;
  ledStrip * light;
  unsigned int autoTurnOff=300000;

  bool wasSensorOpen;
  
  public:
  closetLight(doorSensor * s, ledStrip * l) {
    sensor = s;
    light = l;
  }

  public:
  void Update() {
    sensor->Update();

// state changed
    if (wasSensorOpen != sensor->isOpen() && millis()-sensor->statusChangedAt() >= 1000) {
      wasSensorOpen = sensor->isOpen();
      if (sensor->isOpen() == true) {
        if (light->isOn() == false) {
          light->turnOn();
        }
      }
      else {
        if (light->isOn() == true)
          light->turnOff();
      }
    }

    if (light->isOn() == true && millis()-light->getStateChangedAt() >= autoTurnOff) {
      Serial.print("Auto turnoff light ");
      light->turnOff(); 
    }

  }
};

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_RGB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

const int SENSOR_LEFT_PIN = 2;
const int SENSOR_RIGHT_PIN = 3;


doorSensor leftSensor(2);
ledStrip leftLight("links", &strip, 0, 60);

doorSensor rightSensor(3);
ledStrip rightLight("rechts", &strip, 60, 120);

closetLight leftCloset(&leftSensor, &leftLight);
closetLight rightCloset(&rightSensor, &rightLight);

unsigned long timer0;


void setup() {
  Serial.begin(9600);
//  strip.begin();
//  strip.show(); // Initialize all pixels to 'off'

//  pinMode(SENSOR_LEFT, INPUT_PULLUP);
//  pinMode(SENSOR_RIGHT, INPUT_PULLUP);
//
//  debounceLeft.attach(SENSOR_LEFT);
//  debounceLeft.interval(15);
//  
//  debounceRight.attach(SENSOR_RIGHT);
//  debounceRight.interval(15);

  
  Serial.println("Running.....");
  timer0 = millis();

  Serial.print("left sensor pin ");
  Serial.println(leftSensor.getPin());

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}



void loop() {
  leftCloset.Update();
  rightCloset.Update();
}

//void handleActor(int sensor, bool state) {
//  Serial.println("**** HANDLE ACTOR");
//  uint8_t start;
//  uint32_t color;
//  uint8_t number;
//
//  number = (int) (strip.numPixels()/2);
//  if (sensor == SENSOR_LEFT) {
//    uint32_t start = 0;
//  } else {
//    uint32_t start = number;
//  }
//
//  
//  if (state)
//    color = strip.Color(0,0,0);
//  else
//    color = strip.Color(255,255,255);
//
//  colorWipe(color, 50, start, number);
//  
//}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void colorWipe(uint32_t c, uint8_t wait, uint8_t start, uint8_t number) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
  uint8_t stopAt = start+number;
  for(uint16_t i=start; i<stopAt; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

