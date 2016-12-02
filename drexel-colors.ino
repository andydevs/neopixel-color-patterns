/**
 * Program: Drexel Colors
 * 
 * Color displays with Drexel Colors!
 * 
 * Author:  Anshul Kharabanda
 * Created: 9 - 10 - 2016
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

// Color info
Adafruit_ColorRGB blue    = Adafruit_ColorRGB(0, 0, 255);
Adafruit_ColorRGB yellow  = Adafruit_ColorRGB(255, 255, 0);
Adafruit_GradientRGB fade = Adafruit_GradientRGB(blue, yellow, 256);

/**
 * Runs at the beginning of code
 */
void setup() {
  Serial.begin(9600);
  strip.begin();
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
    colorAlternate(blue, yellow, 4, ALT_TIME);
  }
  
  // Gradient fade 3 times
  for (int i = 0; i < 3; i++) {
    gradientFade(blue, fade, FADE_TIME);
  }

  // Cascade fade 5 times
  for (int i = 0; i < 5; i++) {
    gradientCascadeFade(blue, fade, CASCADE_FADE_TIME);
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
void gradientFade(Adafruit_ColorRGB start, Adafruit_GradientRGB gradient, uint16_t wait) {
  uint16_t x;
  for (int i = 0; i < 2*gradient.total; i++) {
    for (int j = 0; j < strip.numPixels(); j++) {
      x = i > gradient.total ? 2*gradient.total - i : i;
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
void gradientCascadeFade(Adafruit_ColorRGB start, Adafruit_GradientRGB gradient, uint16_t wait) {
  uint16_t l, x;
  for (int i = 0; i < 2*gradient.total; i++) {
    for (int j = 0; j < strip.numPixels(); j++) {
      l = (i + j*gradient.total/strip.numPixels()) % (2*gradient.total);
      x = l > gradient.total ? 2*gradient.total - l : l;
      strip.setPixelColor(j, start + gradient*x);
    }
    strip.show();
    delay(wait);
  }
}

