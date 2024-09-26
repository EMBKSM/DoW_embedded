#include <Wire.h>               
#include <Adafruit_GFX.h>      
#include <Adafruit_SSD1306.h>      
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH 128            
#define SCREEN_HEIGHT 64             

#define OLED_RESET     4             
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

using namespace std;

const char* ssid = "your_SSID";         // WiFi SSID
const char* password = "your_PASSWORD"; // WiFi 비밀번호

const char* server = "your_server.com"; // 서버 도메인 또는 IP 주소
const int port = 80;                    // 서버 포트 (HTTP 기본 포트는 80)
const char* path = "/embedded/connect";      // SSE가 제공되는 경로

WiFiClient client;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 32400, 60000);

String jwt_token ="0";
int Alarm_time[21];
array<String, 21> title;
int cnt_1 = 0;
int cnt_2 = 0;
int cnt = 0;

void setup() {
  timeClient.begin();

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
  if(jwt_token !="0"){
    if (client.connect(server, port)) {
    Serial.println("서버에 연결되었습니다.");
    client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "jwt_token: " + jwt_token + "\r\n" +
                 "Accept: text/event-stream\r\n" + 
                 "Connection: keep-alive\r\n\r\n");
    }
  }
  else{
    if (client.connect(server, port)) {
    Serial.println("서버에 연결되었습니다.");
    client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Accept: text/event-stream\r\n" + 
                 "Connection: keep-alive\r\n\r\n");
    }
  }
  
  while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
}

void loop(){
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();
  int hours = currentTime.substring(0, 2).toInt();
  int minutes = currentTime.substring(3, 5).toInt()+ (hours*60);
  if (client.available()) {
    if(cnt_1 == cnt_2){
      cnt++;
    }
    String line = client.readStringUntil('\n');
    if(jwt_token == "0"){
      if (line.startsWith("jwt_token: ")) {
        String eventData = line.substring(11);
        jwt_token = eventData;
      }
    }
    if (line.startsWith("title: ")) {
      String eventData = line.substring(7); 
      title.at(cnt%20) = eventData;
      cnt_1++;
    }
    if (line.startsWith("date: ")) {
      String eventData = line.substring(6); 
      Alarm_time[cnt%20] = strtoul(eventData.c_str(), NULL, 10);
      cnt_2++;
    }
  }// 밀리초를 분으로 바꾸기
  if(minutes >= Alarm_time[0]){
    digitalWrite(D7,HIGH);
  }
}
