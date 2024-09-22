#include <Wire.h>               
#include <Adafruit_GFX.h>      
#include <Adafruit_SSD1306.h>       

#define SCREEN_WIDTH 128            
#define SCREEN_HEIGHT 64             

#define OLED_RESET     4             
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  const char* ssid = "name";     // wifi 이름
  const char* password="password";  // wifi 패스워드
  const char* serverAddress = "http://255.255.0.0:8080/plant_m/connect"; // 서버주소
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
  
}

void loop() {

}
