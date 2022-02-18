#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include<ArduinoJson.h>
#include<string.h>
#include<esp_task_wdt.h>
#include<math.h>
int ledPin= 13;
int inputPin= 32;
int ledPout= 12;
int inputPout= 33;
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
const char* ssid = "Tannie";
const char* password = "TanZaKunG1";

const char* url_post  = "https://ecourse.cpe.ku.ac.th/exceed01/api/stamp04/2/update";
const char* url_post2 = "https://ecourse.cpe.ku.ac.th/exceed01/api/stamp04/2/get_number_now_sit";
static int save;
char str[100];
const int _size = 2*JSON_OBJECT_SIZE(4);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;
void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(ledPout, OUTPUT);
  pinMode(inputPout, INPUT);
  lcd.begin();
  lcd.backlight();
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);  
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}
void _post_in(){
    
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url_post);
        http.addHeader("Content-Type","application/json");
        /*JSONPost["room"] = room;
        if(room==1)
            JSONPost["status"]  = (int)digitalRead(led);
        else if (room == 2)
            JSONPost["status"]  = (int)digitalRead(led1);
        else if (room == 3)
            JSONPost["status"]  = (int)digitalRead(led2);
            
        */
        JSONPost["sensor_in"]  = (int)digitalRead(32);
        JSONPost["sensor_out"]  = (int)digitalRead(33);
        serializeJson(JSONPost,str);
        int httpCode = http.POST(str);
        if(httpCode == HTTP_CODE_OK){
            String payload = http.getString();
            Serial.println(httpCode);
            //  lcd.setCursor(0, 0);
            //lcd.print(payload);
            Serial.println(payload);
        }
        else{
            Serial.println(httpCode);
            Serial.println("ERROR on HTTP Request");
        }
    }
    delay(100);
}
void _post_out(){
    
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url_post2);
        http.addHeader("Content-Type","application/json");
        /*JSONPost["room"] = room;
        if(room==1)
            JSONPost["status"]  = (int)digitalRead(led);
        else if (room == 2)
            JSONPost["status"]  = (int)digitalRead(led1);
        else if (room == 3)
            JSONPost["status"]  = (int)digitalRead(led2);
            
        */
      
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
            String payload = http.getString();
            Serial.println(httpCode);
            deserializeJson(JSONGet,payload);
            save = (int)JSONGet["now_sit"];
            
            //  lcd.setCursor(0, 0);
            //lcd.print(payload);
            Serial.println(save);
        }
        else{
            Serial.println(httpCode);
            Serial.println("ERROR on HTTP Request");
        }
    }
    delay(100);
}





void loop(){
  int value1= digitalRead(inputPin);
  int value2= digitalRead(inputPout);
  //count poeple in// 
  Serial.print("post_in");
  _post_in();
  Serial.print("post_out");
  _post_out();
  if (value1 == HIGH) 
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }

  //count poeple out//   
  if (value2 == HIGH)
  {
    digitalWrite(ledPout, HIGH);
  }
  else
  {
    digitalWrite(ledPout, LOW);
  } 
}
