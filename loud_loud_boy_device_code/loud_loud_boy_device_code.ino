#include <Wire.h>               
#include <Adafruit_GFX.h>      
#include <Adafruit_SSD1306.h>      
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SCREEN_WIDTH 128            
#define SCREEN_HEIGHT 64             

#define OLED_RESET     4             
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const char* ssid = "name";     // wifi 이름
const char* password="password";  // wifi 패스워드
const char* serverAddress = "http://255.255.0.0:8080/plant_m/connect"; // 서버주소
int time=0;
int Server_time;

void setup() {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, OUTPUT);
}

void loop() {
  time = (millis()/60000)%1440;
}
