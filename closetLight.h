#include <Arduino.h>
#include <Bounce2.h>
#include <Adafruit_NeoPixel.h>


class closetLight {
  public:
  
    closetLight(int s, int f, int l, Adafruit_NeoPixel *str, unsigned long t);
    ~closetLight();
    void init();
    void process();
    


  private:
    int sensorPin;
    int firstLed;
    int lastLed;
    Adafruit_NeoPixel *strip;
    Bounce debouncer;
    bool currentState;
    uint32_t color;
    int ledPos;
    unsigned long onAt;
    unsigned long timeOut;

    void initLed();

};

