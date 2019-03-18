#include <dht11.h>
#include <Servo.h>
#include <FastLED.h>
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define ValvePin 4
#define DHT11PIN 5

#define LED_TYPE         WS2812     // Модель ленты
#define COLOR_ORDER      GRB        // Цветовая модель
#define PIN              5         // К какому выходу подключена лента
#define NUM_LEDS         12         // Кол-во светодиодов в ленте
#define COLOR_CORRECTION 0xFFB0F0   // Цветокоррекция
#define BRIGHTNESS       255        // Яркость ленты
CRGB leds[NUM_LEDS];


dht11 DHT;
Servo valveservo;
int input;
void setup()
{
  Serial.begin(9600);
  valveservo.attach(ValvePin);
  FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.setBrightness(BRIGHTNESS);
}

void fill(){
  for(int i=1;i<12;i++){
  leds[i]+= CHSV(random8(64), random8(64), random8(64));
  }
}




void loop()
{
  Serial.println();
  fill();
  FastLED.show();
      if(Serial.available() > 0)
    {
        input = Serial.parseInt();
    }
  if(input>180){input=180;}
   if(input<0){input=0;}
   valveservo.write(input);
  Serial.println(input);
  int chk = DHT.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT.humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((float)DHT.temperature, 2);

  delay(2000);

}
