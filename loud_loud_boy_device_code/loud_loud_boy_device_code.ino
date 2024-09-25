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
int pre_time=0;
int Server_time;
int Alarm_time[20];
int cnt=0;


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
  pinMode(D8, INPUT);
  if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      WiFiClient client;
      http.begin(client, serverAddress); 
      if()

      http.end();
  }
}

void loop() {
  pre_time = Server_time + (millis()/60000)%1440;// 밀리초를 분으로 바꾸기
  if(pre_time >= Alarm_time[cnt%20]){
    digitalWrite(D7,HIGH);
  }
}
