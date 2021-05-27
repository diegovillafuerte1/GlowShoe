// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define SENSORPIN 0 //analog pin 0

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      5

  // Generally, you should use "unsigned long" for variables that hold time
  // The value will quickly become too large for an int to store
  unsigned long previousMillis = 0;        // will store last time LED was updated
  unsigned long rainbowStartMillis = 6000;
  // constants won't change:
  const long interval = 20;

float brightness = 1.0; 
uint32_t strip_state=0; 

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 200; // delay for half a second
//int lastPressedValue;
int wheelEnded = 0;
void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(SENSORPIN, INPUT);
//  lastPressedValue = !(digitalRead(SENSORPIN));
  Serial.begin(9600);
}

void loop() {
  
//  Serial.println(digitalRead(SENSORPIN));

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  int isSensorPressed = !digitalRead(SENSORPIN);
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    if(isSensorPressed == 0){
      
      Serial.println("rainbowstartMillis");
      Serial.println(rainbowStartMillis);
      rainbowStartMillis = rainbowStartMillis - delayval;
      if(rainbowStartMillis <5000){
        Serial.println("starting white flash");
        for(int i=0;i<NUMPIXELS;i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(255,255,255)); // Moderately bright white color.
        }
        pixels.show();
        rainbowStartMillis = 4000;
      }
      else if(rainbowStartMillis > 5000){
        rainbow2();
      }
    }
    else if (isSensorPressed == 1){
//      Serial.println("rainbow func");
      rainbowStartMillis = 6000;
      rainbow2();
    }
    
  }

      

  delay(delayval); // Delay for a period of time (in milliseconds).
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow() {
  for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
    pixels.setPixelColor(i + strip_state % 3, Wheel(&pixels, (i+strip_state) % 255));    //turn every third pixel on
    pixels.setPixelColor(i + (strip_state +1) % 3, 0); // turn others off
    pixels.setPixelColor(i + (strip_state +2) % 3, 0);
  }
  pixels.show();
  strip_state++;
}

void rainbow2() {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(&pixels,(i+strip_state) & 255));
    }
    pixels.show();
  }
//    Serial.println("strip state");
//  Serial.println(strip_state);
    strip_state+= 30;
}
void moving_rainbow(){
  for(unsigned int i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel(&pixels,((i * 256 / pixels.numPixels()) + strip_state) & 255));
  }

  pixels.show();
  strip_state++;
}


uint32_t Wheel(Adafruit_NeoPixel *strip, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip->Color((255 - WheelPos * 3)*brightness, 0, WheelPos * 3*brightness);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, WheelPos * 3 * brightness, (255 - WheelPos * 3)*brightness);
  }
  WheelPos -= 170;
  return strip->Color(WheelPos * 3 * brightness, (255 - WheelPos * 3)*brightness, 0);
}
