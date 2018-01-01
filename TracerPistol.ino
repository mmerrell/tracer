#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Constants
#define DELAY_INTERVAL 20

//Ring values
#define LEFT_RING_PIN        5 
#define REAR_LIGHT_PIN       10
#define FRONT_RUNNER_PIN     11
Adafruit_NeoPixel left_ring  = Adafruit_NeoPixel(16, LEFT_RING_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel right_ring = Adafruit_NeoPixel(16, RIGHT_RING_PIN, NEO_GRB + NEO_KHZ800);

#define RING_MIN_BRIGHTNESS 10
#define RING_MAX_BRIGHTNESS 190
#define PIXEL_INTERVAL      8

int current_pixel_left;
int current_pixel_right;
int outer_loop_index;

//Fixed light values
#define INITIAL_BRIGHTNESS   64
#define MAX_BRIGHTNESS       255
#define FADE_INTERVAL        10

int current_brightness;
boolean fade_in;

#define LOGGING false

void setup() {
  if (LOGGING) {
    Serial.begin(9600);
  }

  //initialize NeoPixel Rings
  left_ring.begin();
  left_ring.show();
  right_ring.begin();
  right_ring.show();

  //initialize fixed lights
  pinMode(REAR_LIGHT_PIN, OUTPUT);
  pinMode(FRONT_RUNNER_PIN, OUTPUT);

  //initialize fixed light variables
  current_brightness = INITIAL_BRIGHTNESS;
  fade_in = true;

  //initialize ring variables
  outer_loop_index = 0;
}

void loop() {
  current_brightness = update_fixed_lights(current_brightness);
  update_rings();
  delay(DELAY_INTERVAL);
}

//This method updates the brightness on all the "fixed" lights (rear/front site, anterior running lights)
int update_fixed_lights(int brightness) {
  if (fade_in) {
    brightness += FADE_INTERVAL;
  } else {
    brightness -= FADE_INTERVAL;
  }

//  Serial.print("Current Brightness: ");
//  Serial.println(brightness);

  //We've hit MAX, so toggle fade_in
  if (brightness > MAX_BRIGHTNESS) {
    fade_in = !fade_in;
//    Serial.println("Fading out: reset to MAX and short-circuit to next iteration...");
    return MAX_BRIGHTNESS;
  }

  //We've hit MIN, so toggle fade_in
  if (brightness < INITIAL_BRIGHTNESS) {
    fade_in = !fade_in;
//    Serial.println("Fading in...: ");
  }

//  Serial.print("New Brightness: ");
//  Serial.println(brightness);

  //finally, write the value to the pins
  analogWrite(REAR_LIGHT_PIN, brightness);
  analogWrite(FRONT_RUNNER_PIN, brightness);

  return brightness;
}

void update_rings() {
//  Serial.println("In rings()");
  //pixel ring

  if (outer_loop_index <= PIXEL_INTERVAL) {
//    Serial.println("In outer loop");
    for (uint16_t pixel_index = 0; pixel_index < left_ring.numPixels(); pixel_index = pixel_index + PIXEL_INTERVAL) {
//      Serial.print("Lighting pixel #");
//      Serial.println(i+outer_loop_index);
      right_ring.setPixelColor(pixel_index+outer_loop_index, right_ring.Color(0, 0, RING_MAX_BRIGHTNESS)); //clockwise
      left_ring.setPixelColor(left_ring.numPixels() - pixel_index - outer_loop_index, left_ring.Color(0, 0, RING_MAX_BRIGHTNESS)); //counter-clockwise
    }

    right_ring.show();
    left_ring.show();
    delay(DELAY_INTERVAL);
    
    for (uint16_t pixel_index = 0; pixel_index < left_ring.numPixels(); pixel_index = pixel_index + PIXEL_INTERVAL) {
//      Serial.print("Disabling pixel #");
//      Serial.println(i+outer_loop_index);
      right_ring.setPixelColor(pixel_index + outer_loop_index, right_ring.Color(0, 0, RING_MIN_BRIGHTNESS)); //clockwise
      left_ring.setPixelColor(left_ring.numPixels() - pixel_index - outer_loop_index, left_ring.Color(0, 0, RING_MIN_BRIGHTNESS)); //counter-clockwise
    }
    right_ring.show();
    left_ring.show();

  } else {
      outer_loop_index = 0;
  }
  outer_loop_index++;
}

//void log(String message) {
//  print(message);  
//}
    


