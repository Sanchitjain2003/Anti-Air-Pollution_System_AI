#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D5,D6);
const char* ssid = "Home";
const char* password = "Lagan1998";

void setup() 
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  s.begin(9600);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting...");
  }
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://api.airvisual.com/v2/city?city=Ghaziabad&state=Uttar%20Pradesh&country=India&key=2c23aa7d-e9ed-4485-97ff-98207407a347");
    int httpCode = http.GET();

    if (httpCode > 0) 
    {
      const size_t capacity = JSON_ARRAY_SIZE(2) + 3*JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(7) + 320;
DynamicJsonBuffer jsonBuffer(capacity);

const char* json = "{\"status\":\"success\",\"data\":{\"city\":\"Ghaziabad\",\"state\":\"Uttar Pradesh\",\"country\":\"India\",\"location\":{\"type\":\"Point\",\"coordinates\":[77.354558,28.653893]},\"current\":{\"weather\":{\"ts\":\"2020-06-28T13:00:00.000Z\",\"tp\":40,\"pr\":996,\"hu\":45,\"ws\":4.6,\"wd\":250,\"ic\":\"50d\"},\"pollution\":{\"ts\":\"2020-06-28T13:00:00.000Z\",\"aqius\":121,\"mainus\":\"p1\",\"aqicn\":123,\"maincn\":\"p1\"}}}}";

JsonObject& root = jsonBuffer.parseObject(json);

const char* status = root["status"]; // "success"

JsonObject& data = root["data"];
const char* data_city = data["city"]; // "Ghaziabad"
const char* data_state = data["state"]; // "Uttar Pradesh"
const char* data_country = data["country"]; // "India"

const char* data_location_type = data["location"]["type"]; // "Point"

float data_location_coordinates_0 = data["location"]["coordinates"][0]; // 77.354558
float data_location_coordinates_1 = data["location"]["coordinates"][1]; // 28.653893

JsonObject& data_current_weather = data["current"]["weather"];
const char* data_current_weather_ts = data_current_weather["ts"]; // "2020-06-28T13:00:00.000Z"
int data_current_weather_tp = data_current_weather["tp"]; // 40
int data_current_weather_pr = data_current_weather["pr"]; // 996
int data_current_weather_hu = data_current_weather["hu"]; // 45
float data_current_weather_ws = data_current_weather["ws"]; // 4.6
int data_current_weather_wd = data_current_weather["wd"]; // 250
const char* data_current_weather_ic = data_current_weather["ic"]; // "50d"

JsonObject& data_current_pollution = data["current"]["pollution"];
const char* data_current_pollution_ts = data_current_pollution["ts"]; // "2020-06-28T13:00:00.000Z"
int data_current_pollution_aqius = data_current_pollution["aqius"]; // 121
const char* data_current_pollution_mainus = data_current_pollution["mainus"]; // "p1"
int data_current_pollution_aqicn = data_current_pollution["aqicn"]; // 123
const char* data_current_pollution_maincn = data_current_pollution["maincn"]; // "p1

      Serial.print("AQI:");
      Serial.println(data_current_pollution_aqius);
      /*Serial.print("Username:");
      Serial.println(username);
      Serial.print("Email:");
      Serial.println(email);*/
      if(s.available()>0)
{
 s.write(121);
}
    }
    http.end(); //Close connection
  }
  delay(60000);
}
