#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50*2, PIN, NEO_GRB + NEO_KHZ800);
uint32_t black = strip.Color(0,00,0);
uint8_t *pixels = strip.getPixels();
uint8_t header[2];

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  //digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);               // wait for a second
  //Serial.println("Test");
  if (Serial.available() > 0)
  {
    int count = Serial.readBytes((char*)header, 2);
    if (count != 2) return;
    //Header contains the string id and the size of pixels to set
    uint16_t size = header[1]*3; //RGB per pixel
    strip.setPin(header[0]);
    count = Serial.readBytes((char*)pixels, size);
    if (count == size)
    {
      strip.show();
      Serial.print("A");
    } else {
      Serial.print("N");
    }
  }
}


