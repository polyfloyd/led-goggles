// https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

const int dataPin = 6; // D6
const int pixels = 16 * 2;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixels, dataPin, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(9600);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

    strip.setBrightness(16); // Oof owie my eyes hurt
}

void loop() {
    hypnotoad(6000);
    infinityLoop(6000);
    rainbow2D(6000);
    larsonScanner(6000);
    chaos35(6000);
    rainbow(4000);
    spinRedWhite(4000);
    police(2000);

//    scanners(2000);
}

void hypnotoad(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < 32; i++) {
            uint8_t x, y;
            xy(i, &x, &y);

            long phase = 255 * (i<16);
            int16_t tt = abs((long)(t/2 + phase) % 511 - 255);
            uint8_t core = max(0, (256 - abs(y - 127)) - tt);

            uint8_t flash = 255 * (core/8) * (t%300 < 10);
            uint32_t c = strip.Color(core, (core>>1) * (core<127), flash);
            strip.setPixelColor(i, c);
        }
        strip.show();
    }
}

void infinityLoop(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < 32; i++) {
            uint16_t o;
            if (i < 16) {
                o = 15 - (i + 3) % 16;
            } else {
                o = (i + 12) % 16 + 16;
            }
            uint32_t c = hueRotate(((o + t/32) >> 5) * 89);
            strip.setPixelColor(i, c);
        }
        strip.show();
    }
}

void larsonScanner(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < 32; i++) {
            uint8_t x, y;
            xy(i, &x, &y);

            const uint8_t w = 32;

            int16_t j = abs((long)t % 511 - 255);
            int16_t k = (x+w/2 + j) - 256;
            uint8_t a = (k > 0 && k < w) * 0xff;

            uint32_t c = strip.Color(a, 0, 0);
            strip.setPixelColor(i, c);
        }
        strip.show();
    }
}

void rainbow2D(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < 32; i++) {
            uint8_t x, y;
            xy(i, &x, &y);
            uint8_t a = x + t / 5;
            strip.setPixelColor(i, hueRotate(a));
        }
        strip.show();
    }
}

void chaos35(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < 32; i++) {
            uint8_t x, y;
            xy(i, &x, &y);
            uint8_t a = abs(((long)x + (long)t / 4) % 511 - 255);
            uint32_t c = strip.Color(0, mix(0, 255, a), mix(255, 0, a));
            strip.setPixelColor(i, c);
        }
        strip.show();
    }
}

void scanners(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < 16; i++) {
            int32_t a = (i - t / 80) % 16 - 7;
            int32_t b = (i + t / 50) % 16 - 11;
            uint32_t j = (a >= 0) * 0xff;
            uint32_t k = (b >= 0) * 0xff;
            uint32_t c = strip.Color(j, k, 0);
            strip.setPixelColor(i, c);
            strip.setPixelColor(i + 16, c);
        }
        strip.show();
    }
}

void police(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < 16; i++) {
            uint16_t a = (t / 100) & 1;
            strip.setPixelColor(i, strip.Color(a * 0xff, 0, !a * 0xff));
            strip.setPixelColor(i + 16, strip.Color(!a * 0xff, 0, a * 0xff));
        }
        strip.show();
    }
}

void rainbow(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        uint32_t offset = t / 5;
        for (uint16_t i = 0; i < 16; i++) {
            strip.setPixelColor(i, hueRotate(i * 16 + offset));
            strip.setPixelColor(i+16, hueRotate(i * 16 - offset));
        }
        strip.show();
    }
}

void spinRedWhite(unsigned long duration_ms) {
    unsigned long t, until = millis() + duration_ms;
    while ((t = millis()) < until) {
        for (uint16_t i = 0; i < strip.numPixels(); i++) {
            uint32_t a = (i + t / 60) / 4;
            uint32_t c = strip.Color(0xff, (a&1)*0xff, (a&1)*0xff);
            strip.setPixelColor(i, c);
        }
        strip.show();
    }
}

void xy(uint16_t i, uint8_t *x, uint8_t *y) {
    float correction = (-PI / 8) * (i < 16);
    float angrad = ((float)i / 16.0) * PI*2 + PI/2 + correction;
    *x = round(cos(angrad) * 63.0) + 64 + (i / 16) * 127;
    *y = round(sin(angrad) * 127.0) + 128;
}

uint8_t mix(uint16_t x, uint16_t y, uint16_t a) {
    return (x * (0xff - a)  + y * a) >> 8;
}

uint32_t hueRotate(uint8_t hue) {
    hue = 255 - hue;
    if (hue < 85) {
        return strip.Color(255 - hue * 3, 0, hue * 3);
    } else if(hue < 170) {
        hue -= 85;
        return strip.Color(0, hue * 3, 255 - hue * 3);
    } else {
        hue -= 170;
        return strip.Color(hue * 3, 255 - hue * 3, 0);
    }
}
