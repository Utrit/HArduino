#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <dht11.h>
#include <Servo.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define ValvePin 4 //d2
#define DHT11PIN 5 //d1
#define WPin     0 //a0

#define LED_TYPE         WS2812     // Модель ленты
#define COLOR_ORDER      GRB        // Цветовая модель
#define PIN              14         // К какому выходу подключена лента
#define NUM_LEDS         12         // Кол-во светодиодов в ленте
#define COLOR_CORRECTION 0xFFB0F0   // Цветокоррекция
#define BRIGHTNESS       100        // Яркость ленты
#ifndef STASSID
#define STASSID "Redmi Note 3"
#define STAPSK  "vadimpopov3"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

CRGB leds[NUM_LEDS];
dht11 DHT;
Servo valveservo;
long WatSoil,TAir,HumAir,LastWat,rt;
float t;
void setup()
{
  Serial.begin(115200);
  valveservo.attach(ValvePin);
  FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.setBrightness(BRIGHTNESS);
  conn();
  for(int i=0;i<10;i++){
  if(rt==0){
  rt = getservertime();
  }
  }
}
void loop()
{
  rt++;
  fill();
  FastLED.show();
  DHT.read(DHT11PIN);
  HumAir = DHT.humidity;
  TAir = DHT.temperature;
  WatSoil = analogRead(WPin);
  if(WatSoil<550){
    valveservo.write(90);
    LastWat=rt;
  }else
  {valveservo.write(0);
   
    };
  Serial.println(rt);
  Serial.println(TAir);
  Serial.println(HumAir);  
  Serial.println(WatSoil);    
  Serial.println(LastWat);    
  sendtoserver();  
  delay(1000);
}



void sendtoserver(){
  WiFiClient client;
  HTTPClient http;
      Serial.print("[HTTP] begin...\n");
      //Id=1&Ha=2&Ta=3&Hs=4&Li=5&Wt=6
      String HR = "http://192.168.43.11/hsite/datafrom.php?";
      HR+="Id="+1;
      HR+="&Ha="+HumAir;
      HR+="&Ta="+TAir;
      HR+="&Hs="+WatSoil;
      HR+="&Li="+rt;
      HR+="&Wt="+LastWat;
      Serial.println(HR);  
    if (http.begin(client, "http://192.168.43.11/HSite/time.php")) {  // HTTP
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      }
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }


  
}

void fill() {
  t = rt/86400*3.14;
  for (int i = 0; i < NUM_LEDS; i++) {
    FastLED.setBrightness(sin8(t));
    float r = sin(t + i * 0.03 + 1) + 1;
    float g = sin(t + i * 0.03 + 2) + 1;
    float b = sin(t + i * 0.03 + 3) + 1;
    leds[i] = CRGB(r * 127, g * 127, b * 127);
  }
}

void conn() {
    Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
long getservertime(){
  WiFiClient client;
  HTTPClient http;
      Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://192.168.43.11/HSite/time.php")) {  // HTTP
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          return payload.toFloat();
        }
      }
      return http.getString().toFloat();
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
      return 0;
    }
}

