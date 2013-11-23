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

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

	pinMode(13, OUTPUT);
}

void loop() {

	//digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	//delay(1000);               // wait for a second
	if (Serial.available() > 0)
	{
    Serial.print("Aval ");
    Serial.println(Serial.available());
		int id = Serial.read();
		uint16_t size = Serial.read()*3;
		strip.setPin(id);
		int count = Serial.readBytes((char*)pixels, size);
		if (count == size)
		{
			strip.show();
			Serial.print("OK ");
			Serial.println(count);
		}
		//{
		//	strip.show();
		//	//digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
		//	//delay(1000);
		//}
	}
	//if (Serial.available() > 0)
	//{
	//	//int id = Serial.read();
	//	//uint16_t size = Serial.read()*3;
	//	strip.setPin(2);
	//	//int count = Serial.readBytes((char*)pixels, size);
  //  for(int i=0; i<50*3; i++)
	//	{
	//			pixels[i] = 0;
	//	}
	//		strip.show();
	//		//digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	//		//delay(1000);
	//}

}

//void clear() {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, black);
//  }
//   strip.show();
//}

