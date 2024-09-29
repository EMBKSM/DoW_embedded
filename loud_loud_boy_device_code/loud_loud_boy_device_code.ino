#include <Wire.h>               
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define SCREEN_WIDTH 128            
#define SCREEN_HEIGHT 64             

#define OLED_RESET     4             

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
String Alarm_time_for_screen[21];
String title[21];
uint8_t cnt_1 = 0;
uint8_t cnt_2 = 0;
uint8_t cnt_3 = 0;
uint8_t cnt = 0;
uint8_t buzzer_on = 0;
uint8_t page = 0;


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
  timeClient.begin();

  Serial.begin(115200);

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
  u8g2.clearBuffer();
  timeClient.update();



  String currentTime = timeClient.getFormattedTime();
  int hours = currentTime.substring(0, 2).toInt();
  int minutes = currentTime.substring(3, 5).toInt()+ (hours*60);

  
  screen_print(page);
  
  server_receive();

  if(minutes >= Alarm_time[0]){
    digitalWrite(D7,HIGH);
    buzzer_on = 1;
  }
  if(buzzer_on == 1){
    if(digitalRead(D8) == HIGH){
    digitalWrite(D7,LOW);
    }
  }
  u8g2.sendBuffer();

}

void screen_print(uint8_t n){
  u8g2.setFont(u8g2_font_6x10_tf);  
  u8g2.drawStr(0, 0, return_char_Array(Alarm_time_for_screen[n]));
  u8g2.setFont(u8g2_font_unifont_t_korean1);  
  u8g2.drawStr(0, 8, return_char_Array(title[n].substring(0, 8)));
  u8g2.drawStr(0, 24, return_char_Array(title[n].substring(8,16)));
  u8g2.drawStr(0, 40, return_char_Array(title[n].substring(16,24)));
}

void server_receive(){
  if (client.available()) {
    if(cnt_1 == cnt_2&& cnt_2 == cnt_3){
      if(cnt == 239){
        cnt = 0;
        cnt_1 = 0;
        cnt_2 = 0;
        cnt_3 = 0;
      }
    }
    String line = client.readStringUntil('\n');
    if(jwt_token == "0"){
      if (line.startsWith("jwt_token: ")) {
        String eventData = line.substring(11);
        jwt_token = eventData;
      }
    }
    else if (line.startsWith("title: ")) {
      String eventData = line.substring(7); 
      title[cnt%20] = eventData;
      cnt_1++;
    }
    else if (line.startsWith("time_data: ")) {
      String eventData = line.substring(11); 
      Alarm_time[cnt%20] = strtoul(eventData.c_str(), NULL, 10);
      cnt_2++;
    }
    else if (line.startsWith("date: ")) {
      String eventData = line.substring(6); 
      Alarm_time_for_screen[cnt%20] = eventData;
      cnt_3++;
    }
  }
}

char* return_char_Array(String str){
  char* charArray = stringToCharArray(str);
  return charArray;
}


char* stringToCharArray(String input) {
  int length = input.length() + 1; // null 문자 포함
  char* charArray = new char[length]; // 동적 메모리 할당
  input.toCharArray(charArray, length);
  return charArray;
}