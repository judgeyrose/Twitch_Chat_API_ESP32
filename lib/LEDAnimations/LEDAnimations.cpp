#include "LEDAnimations.h"

LEDAnimations::LEDAnimations(Adafruit_NeoPixel& strip) : strip(strip), red(0), green(0), blue(0), brightness(100) {}

void LEDAnimations::setAnimation(String animation) {
    currentAnimation = animation;
}

void LEDAnimations::setColor(uint8_t r, uint8_t g, uint8_t b) {
    red = r;
    green = g;
    blue = b;
}

void LEDAnimations::setBrightness(uint8_t value) {
    brightness = constrain(value, 0, 100);
}

void LEDAnimations::update() {
    if (currentAnimation == "fade") {
        fade();
    } else if (currentAnimation == "blink") {
        blink();
    } else if (currentAnimation == "strobe") {
        strobe();
    } else if (currentAnimation == "pattern") {
        pattern();
    } else if (currentAnimation == "rainbow") {
        rainbow();
    } else {
        strip.setPixelColor(0, strip.Color((red * brightness) / 100, (green * brightness) / 100, (blue * brightness) / 100));
        strip.show();
    }
}

void LEDAnimations::fade() {
    for (int i = 0; i < 256; i++) {
        if (currentAnimation != "fade") {
            red = (red * i) / 255;
            green = (green * i) / 255;
            blue = (blue * i) / 255;
            return;
        }
        strip.setPixelColor(0, strip.Color((red * i * brightness) / (255 * 100), (green * i * brightness) / (255 * 100), (blue * i * brightness) / (255 * 100)));
        strip.show();
        delay(10);
    }
    for (int i = 255; i >= 0; i--) {
        if (currentAnimation != "fade") {
            red = (red * i) / 255;
            green = (green * i) / 255;
            blue = (blue * i) / 255;
            return;
        }
        strip.setPixelColor(0, strip.Color((red * i * brightness) / (255 * 100), (green * i * brightness) / (255 * 100), (blue * i * brightness) / (255 * 100)));
        strip.show();
        delay(10);
    }
}

void LEDAnimations::blink() {
    for (int i = 0; i < 10; i++) {
        if (currentAnimation != "blink") return;
        strip.setPixelColor(0, strip.Color((red * brightness) / 100, (green * brightness) / 100, (blue * brightness) / 100));
        strip.show();
        delay(500);
        strip.setPixelColor(0, strip.Color(0, 0, 0));
        strip.show();
        delay(500);
    }
}

void LEDAnimations::strobe() {
    for (int i = 0; i < 10; i++) {
        if (currentAnimation != "strobe") return;
        strip.setPixelColor(0, strip.Color((red * brightness) / 100, (green * brightness) / 100, (blue * brightness) / 100));
        strip.show();
        delay(50);
        strip.setPixelColor(0, strip.Color(0, 0, 0));
        strip.show();
        delay(50);
    }
}

void LEDAnimations::pattern() {
    for (int i = 0; i < 256; i++) {
        if (currentAnimation != "pattern") {
            red = (red * i) / 255;
            green = (green * (255 - i)) / 255;
            blue = (blue * brightness) / 100;
            return;
        }
        strip.setPixelColor(0, strip.Color((red * i * brightness) / (255 * 100), (green * (255 - i) * brightness) / (255 * 100), (blue * brightness) / 100));
        strip.show();
        delay(20);
    }
}

void LEDAnimations::rainbow() {
    for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
        if (currentAnimation != "rainbow") return;
        for (int i = 0; i < strip.numPixels(); i++) {
            int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
            uint32_t color = strip.gamma32(strip.ColorHSV(pixelHue));
            red = (uint8_t)(color >> 16);
            green = (uint8_t)(color >> 8);
            blue = (uint8_t)color;
            strip.setPixelColor(i, strip.Color((red * brightness) / 100, (green * brightness) / 100, (blue * brightness) / 100));
        }
        strip.show();
        delay(10);
    }
}
