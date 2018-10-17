/**
 * Program: Eagles Colors
 * 
 * Color displays with Eagles Colors!
 * 
 * Author:  Anshul Kharabanda
 * Created: 10 - 15 - 2018
 */

// Neopixel library
#include <Adafruit_NeoPixel.h>

// ----------------------------------- PARAMETERS ------------------------------------

// Time delay between pixels in colorWipe section
const int WIPE_TIME = 20;

// Time delay between each alternation in colorAlternate section
const int STRIPE_TIME = 1000;

// Time delay between fade states in colorFade
const int FADE_TIME = 10;

// Time delay between fade states in colorCascadeFade
const int CASCADE_FADE_TIME = 10;

// ------------------------------------ MAIN CODE ------------------------------------

// Define LED Pin
#define PIN 9

// Create pixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

// Brightness
uint8_t brightness = 255;

// Gradient length
uint16_t gradient_length = 255;

// Color info
Adafruit_ColorRGB blue   = Adafruit_ColorRGB(0.0, 0.0, 1.0);
Adafruit_ColorRGB yellow = Adafruit_ColorRGB(1.0, 1.0, 0.0);

// Gradient info
Adafruit_ColorRGB gradient_blue2yellow = (yellow - blue) / gradient_length;
Adafruit_ColorRGB gradient_yellow2blue = (blue - yellow) / gradient_length;

/**
 * Runs at the beginning of code
 */
void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(brightness);
  strip.show();
}

/**
 * Runs repeatedly
 */
void loop() {
  // Clear color
  colorClear();

  // Wipe each color 3 time
  for (int i = 0; i < 3; i++) {
    colorWipe(blue, WIPE_TIME);
    colorWipe(yellow, WIPE_TIME);
  }

  // Alternate colors 5 times
  for (int i = 0; i < 5; i++) {
    colorStripe(blue, yellow, 4, STRIPE_TIME);
    colorStripe(yellow, blue, 4, STRIPE_TIME);
  }
  
  // Gradient fade 3 times
  for (int i = 0; i < 3; i++) {
    gradientFade(blue, gradient_blue2yellow, gradient_length, FADE_TIME);
    gradientFade(yellow, gradient_yellow2blue, gradient_length, FADE_TIME);
  }

  // Cascade fade 5 times
  for (int i = 0; i < 5; i++) {
    gradientCascadeFade(blue, gradient_blue2yellow, gradient_length, CASCADE_FADE_TIME);
  }
}

// ------------------------------------ EFFECTS ------------------------------------

/**
 * Clears the strip
 */
void colorClear() {
  // Clear color
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

/**
 * Wipes the entire strip with the given color
 * 
 * @param color the color to wipe the strip with
 * @param wait  the wait time between each led lighting
 */
void colorWipe(Adafruit_ColorRGB color, uint16_t wait) {
  // Wipe each color
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

/**
 * Stripe color A and color B in alternating stripes of given width
 * 
 * @param colorA first color
 * @param colorB second color
 * @param width width of stripes
 * @param wait wait time to show colorStripe
 */
void colorStripe(Adafruit_ColorRGB colorA, Adafruit_ColorRGB colorB, uint16_t width, uint16_t wait) {
  // Stripe stride and offset
  uint16_t stride = 2*width;
  uint16_t offset;

  // Stripe color A
  offset = 0;
  for (uint16_t start = offset; start < strip.numPixels(); start += stride) {
    for (uint16_t i = start; i < start + width; i++) {
      strip.setPixelColor(i, colorA);
    }
  }

  // Stripe color B
  offset = width;
  for (uint16_t start = offset; start < strip.numPixels(); start += stride) {
    for (uint16_t i = start; i < start + width; i++) {
      strip.setPixelColor(i, colorB);
    }
  }

  // Show strip
  strip.show();
  delay(wait);
}

/**
 * Fades between colors mapped by gradient
 * 
 * @param gradient the gradient map
 * @param wait     the time between iterations
 */
void gradientFade(Adafruit_ColorRGB start, Adafruit_ColorRGB gradient, uint16_t gradient_length, uint16_t wait) {
  // For each time step
  for (uint16_t t = 0; t < gradient_length; t++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, start + gradient*t);
    }
    strip.show();
    delay(wait);
  }
}

/**
 * Fades a cascade on the strip of colors mapped by the given gradient
 * 
 * @param gradient the gradient map
 * @param wait     the time between iterations
 */
void gradientCascadeFade(Adafruit_ColorRGB start, Adafruit_ColorRGB gradient, uint16_t gradient_length, uint16_t wait) {
  uint16_t l, x;
  for (int i = 0; i < 2*gradient_length; i++) {
    for (int j = 0; j < strip.numPixels(); j++) {
      l = (i + j*gradient_length/strip.numPixels()) % (2*gradient_length);
      x = l > gradient_length ? 2*gradient_length - l : l;
      strip.setPixelColor(j, start + gradient*x);
    }
    strip.show();
    delay(wait);
  }
}
