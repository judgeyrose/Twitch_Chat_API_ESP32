#ifndef LED_ANIMATIONS_H
#define LED_ANIMATIONS_H

#include <Adafruit_NeoPixel.h>

class LEDAnimations {
public:
    LEDAnimations(Adafruit_NeoPixel& strip);
    void setAnimation(String animation);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setBrightness(uint8_t value);
    void update();

private:
    Adafruit_NeoPixel& strip;
    String currentAnimation;
    uint8_t red, green, blue;
    uint8_t brightness;

    void fade();
    void blink();
    void strobe();
    void pattern();
    void rainbow();
};

#endif // LED_ANIMATIONS_H
