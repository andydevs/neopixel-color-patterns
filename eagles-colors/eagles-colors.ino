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
const int ALT_TIME = 1000;

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
uint8_t brightness = 255 ;

// Gradient length
uint16_t gradient_length = 255;

// Color info
Adafruit_ColorRGB green    = Adafruit_ColorRGB(0.0,  0.5,  0.05);
Adafruit_ColorRGB silver   = Adafruit_ColorRGB(0.64, 0.67, 0.68);
Adafruit_ColorRGB gradient = (silver - green) / gradient_length;

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
    colorWipe(green, WIPE_TIME);
    colorWipe(silver, WIPE_TIME);
  }

  // Alternate colors 5 times
  for (int i = 0; i < 5; i++) {
    colorAlternate(green, silver, 4, ALT_TIME);
  }
  
  // Gradient fade 3 times
  for (int i = 0; i < 3; i++) {
    gradientFade(green, gradient, gradient_length, FADE_TIME);
  }

  // Cascade fade 5 times
  for (int i = 0; i < 5; i++) {
    gradientCascadeFade(green, gradient, gradient_length, CASCADE_FADE_TIME);
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
 * Sets each alternating LED to one of two colors
 * 
 * @param colora the first color to set
 * @param colorb the second color to set
 * @param width  the width of one color strip
 */
void colorAlternate(Adafruit_ColorRGB colora, Adafruit_ColorRGB colorb, uint16_t width, uint16_t wait) {
  // Swap boolean
  boolean swap;

  // Stripe A and B
  swap = false;
  for (int i = 0; i < strip.numPixels(); i++) {
    // Swap colors every width amount of pixels
    if (i % width == 0) swap = !swap;

    // Set pixel to the given color
    strip.setPixelColor(i, (swap ? colora : colorb));
  }
  // Update and delay
  strip.show();
  delay(wait);

  // Stripe B and A
  swap = true;
  for (int i = 0; i < strip.numPixels(); i++) {
    // Swap colors every width amount of pixels
    if (i % width == 0) swap = !swap;

    // Set pixel to the given color
    strip.setPixelColor(i, (swap ? colora : colorb));
  }
  // Update and display
  strip.show();
  delay(wait);
}

/**
 * Fades colors mapped by the given gradient
 * 
 * @param gradient the gradient map
 * @param wait     the time between iterations
 */
void gradientFade(Adafruit_ColorRGB start, Adafruit_ColorRGB gradient, uint16_t gradient_length, uint16_t wait) {
  uint16_t x;
  for (int i = 0; i < 2*gradient_length; i++) {
    for (int j = 0; j < strip.numPixels(); j++) {
      x = i > gradient_length ? 2*gradient_length - i : i;
      strip.setPixelColor(j, start + gradient*x);
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
