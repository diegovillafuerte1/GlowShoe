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

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 200; // delay for half a second
int lastPressedValue;
int wheelEnded = 0;
void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(SENSORPIN, INPUT);
  lastPressedValue = !(digitalRead(SENSORPIN));
  Serial.begin(9600);
}

void loop() {
  
  Serial.println(digitalRead(SENSORPIN));

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  int isSensorPressed = !digitalRead(SENSORPIN);
  if(isSensorPressed == lastPressedValue){
    if(wheelEnded == 1){
      rainbow(5);
    }
    //no-op
  }
  else if(isSensorPressed){
      rainbow(5);
  }
  else {
    wheelEnded = 0;
      for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(50,50,50)); // Moderately bright green color.
      }
      
  pixels.show(); // This sends the updated pixel color to the hardware.
  }

  lastPressedValue = isSensorPressed;
  delay(delayval); // Delay for a period of time (in milliseconds).
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
  wheelEnded = 1;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
