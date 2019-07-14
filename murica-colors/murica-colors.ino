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
uint8_t brightness = 100;

// Gradient length
uint16_t gradient_length = 255;

// Color info
#define NUM_COLORS 3
Adafruit_ColorRGB COLORS[NUM_COLORS] = {
  Adafruit_ColorRGB(0.0, 0.0, 1.0),
  Adafruit_ColorRGB(1.0, 1.0, 1.0),
  Adafruit_ColorRGB(1.0, 0.0, 0.0)
};
Adafruit_ColorRGB GRADIENTS[NUM_COLORS];

/**
 * Runs at the beginning of code
 */
void setup() {
  // Generate gradients
  for (int i = 0; i < NUM_COLORS; i++) {
    GRADIENTS[i] = ( COLORS[ (i + 1) % NUM_COLORS ] - COLORS[i] ) / gradient_length;
  }

  // Begin all ports
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
    for (int c = 0; c < NUM_COLORS; c++) {
      wipeColor(c, WIPE_TIME);
    }
  }

  // Alternate colors 5 times
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < NUM_COLORS; j++) {
      stripeAllColors(j, 4, STRIPE_TIME);
    }
  }
  
  // Gradient fade 3 times
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < NUM_COLORS; j++) {
      fadeGradient(j, FADE_TIME);
    }
  }
  
  // Cascade fade 5 times
  for (int i = 0; i < 5; i++) {
    cascadeFadeGradient(CASCADE_FADE_TIME);
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
 * Wipe color at the given index
 * 
 * @param index the index of the color in the COLORS array
 * @param wait delay time between each pixel getting lit up in the wipe
 */
void wipeColor(uint8_t index, uint16_t wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, COLORS[index]);
    strip.show();
    delay(wait);
  }
}

/**
 * Stripes all colors offsetted by the given index offset
 */
void stripeAllColors(uint8_t indexOffset, uint16_t width, uint16_t wait) {
  // Declare variables
  uint16_t stride = NUM_COLORS*width;
  uint16_t c, offset;

  // Stripe each color successively
  for (int k = 0; k < NUM_COLORS; k++) {
     c = (k + indexOffset) % NUM_COLORS;
     offset = k*width;
     stripeColor(c, width, stride, offset);
  }

  // Show strip
  strip.show();
  delay(wait);
}

/**
 * Stripes the color given by the index
 */
void stripeColor(uint8_t index, uint16_t width, uint16_t stride, uint16_t offset) {
  for (uint16_t start = offset; start < strip.numPixels(); start += stride) {
    for (uint16_t i = start; i < start + width; i++) {
      strip.setPixelColor(i, COLORS[index]);
    }
  }
}

/**
 * Fades color/gradient at given index
 * 
 * @param index index of color/gradient combo
 * @param wait wait time between fade frames
 */
void fadeGradient(uint8_t index, uint16_t wait) {
  // Set up start and gradient
  Adafruit_ColorRGB start = COLORS[index];
  Adafruit_ColorRGB gradient = GRADIENTS[index];

  // For each time step
  for (uint16_t t = 0; t < gradient_length; t++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, start + gradient*t);
    }

    // Show strip
    strip.show();
    delay(wait);
  }
}

/**
 * Displays all gradients in a linear cascade 
 * 
 * @param wait delay time between frames
 */
void cascadeFadeGradient(uint16_t wait) {
  // Algorithm variables
  uint16_t gradient_select, true_position, gradient_position;
  Adafruit_ColorRGB start, gradient;

  // For each time t
  for (uint16_t t = 0; t < NUM_COLORS*gradient_length; t++) {
    
    // For each pixel in the strip
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      // Set variables
      true_position = (t + i*gradient_length/strip.numPixels()) % (NUM_COLORS*gradient_length);
      gradient_select = true_position / gradient_length;
      gradient_position = true_position % gradient_length;

      // Set gradients
      start = COLORS[gradient_select];
      gradient = GRADIENTS[gradient_select];

      // Set pixel to gradient
      strip.setPixelColor(i, start + gradient*gradient_position);
    
    }

    // Show strip
    strip.show();
    delay(wait);
    
  }
}
