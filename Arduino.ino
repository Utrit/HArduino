#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <dht11.h>
#include <Servo.h>
#include <FastLED.h>
#define ValvePin 4
#define DHT11PIN 4

#define LED_TYPE         WS2812     // Модель ленты
#define COLOR_ORDER      GRB        // Цветовая модель
#define PIN              14         // К какому выходу подключена лента
#define NUM_LEDS         12         // Кол-во светодиодов в ленте
#define COLOR_CORRECTION 0xFFB0F0   // Цветокоррекция
#define BRIGHTNESS       100        // Яркость ленты
CRGB leds[NUM_LEDS];



dht11 DHT;
Servo valveservo;
int input;
float t;
void setup()
{
  Serial.begin(9600);
  valveservo.attach(ValvePin);
  FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.setBrightness(BRIGHTNESS);
}


//void fill(){
//  uint8_t thishue = millis() * (255 - 200)/255;
//  CHSV hsv;
//  hsv.hue = thishue;
//  hsv.val = 255;
//  hsv.sat = 240;
//  for(int i = 0; i < NUM_LEDS;i++){
//    leds[i] = hsv;
//    hsv.hue += 15;
//  }
//}
void fill() {
  for (int i = 0; i < NUM_LEDS; i++) {
    float r = sin(t + i * 0.03 + 1) + 1;
    float g = sin(t + i * 0.03 + 2) + 1;
    float b = sin(t + i * 0.03 + 3) + 1;
    leds[i] = CRGB(r * 127, g * 127, b * 127);
  }
}


void loop()
{
  t += 0.005;

  Serial.println();
  //FastLED.clear();
  fill();
  FastLED.show();
  //      if(Serial.available() > 0)
  //    {
  //        input = Serial.parseInt();
  //    }
  //  if(input>180){input=180;}
  //   if(input<0){input=0;}
  //   valveservo.write(input);
  Serial.println(t);

  //Serial.print("Humidity (%): ");
  //Serial.println((float)DHT.humidity, 2);

  //Serial.print("Temperature (C): ");
  //Serial.println((float)DHT.temperature, 2);

  delay(20);

}
