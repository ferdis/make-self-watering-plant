#define ANA A0
#define DIGI D5
#define LED D4
#define POMPOUT D6
#define IO_USERNAME "your_username"
#define IO_FEED "feed_name"
#define IO_KEY "your_key"
#define MIN_VAL 250

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "your_ssid";
const char *password = "your_password";

double analogValue = 0.0;
int digitalValue = 0;
double analogVolts = 0.0;
unsigned long timeHolder = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ANA, INPUT);
  pinMode(DIGI, INPUT);
  pinMode(LED, OUTPUT); 
  pinMode(POMPOUT, OUTPUT);
  digitalWrite(LED, HIGH);  // Turn the LED off because the LED is active low

  Serial.begin(115200);
  Serial.println("Connecting to wifi");

  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void loop() {
  HTTPClient http;
  String postData;
  
  analogValue = analogRead(ANA);
  digitalValue = digitalRead(DIGI);
  postData = "value=" + String(analogValue) + "&digital=" + String(digitalValue);
  
  // Serial data
  Serial.print("Analog raw: ");
  Serial.println(analogValue);
  Serial.print("Digital raw: ");
  Serial.println(digitalValue);
  Serial.println(" ");

  http.begin("http://io.adafruit.com/api/v2/" + String(IO_USERNAME) + "/feeds/" + String(IO_FEED) + "/data"); 
  http.addHeader("X-AIO-Key", IO_KEY); 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  
  http.end();

  if (analogValue < MIN_VAL) {
      digitalWrite(LED, HIGH); 
      digitalWrite(POMPOUT, false);
      delay(10000);
  } else {
     Serial.println("PUMPING");
      digitalWrite(LED, LOW); 
      digitalWrite(POMPOUT, true);
      delay(100);
  }
}
