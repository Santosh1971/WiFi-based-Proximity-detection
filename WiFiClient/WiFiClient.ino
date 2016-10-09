
// WiFi Client of Proximity Detection

//Ref:
//https://github.com/esp8266/Arduino/issues/1664
//http://www.esp8266.com/viewtopic.php?f=29&t=2153
//http://internetofhomethings.com/homethings/?p=1270

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid     = "SANTOSH_AP";
const char* password = "watchdog";
const char* host = "http://192.168.4.1";
HTTPClient http;
  char Client_MAC[50];

void rptaSrv(int httpCode){
  if(httpCode == 200) {
           String payload = http.getString();
           Serial.println(payload);
           if(payload=="LED ON "){
            digitalWrite(2, 1);
           }
           else if(payload=="LED OFF "){
            digitalWrite(2, 0);
           }

        } else {
            Serial.print("[HTTP] GET... failed, no connection or no HTTP server\n");
            
            ESP.restart();
       
        }
}

/********************************************************
   ADD Prefix with MAC of WiFi
 ********************************************************/

void AddMAC(char * prefix, char * topic) {
  uint8_t MAC_array[6];
  WiFi.macAddress(MAC_array);
  sprintf(topic, "%s", prefix);
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    sprintf(topic, "%s%02x", topic, MAC_array[i]);
  }
}

/********************************************************/



void setup() {

  Serial.begin(115200);
  delay(10);
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  Serial.println("proyecto inv client-server");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);  //sin esta no funciona
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop(){
        if(WiFi.status() != WL_CONNECTED){
          WiFi.begin(ssid, password);
          delay(500);
        }else{

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url

   
          AddMAC("Client_MAC:", Client_MAC);
          Serial.println(Client_MAC);
          String str1=Client_MAC;


          http.begin("192.168.4.1", 80,( "/myid?state="+ str1)); //HTTp
        int httpCode0 = http.GET();
        rptaSrv(httpCode0);
        delay(500);
          
        http.begin("192.168.4.1", 80, "/led0"); //HTTp
        int httpCode1 = http.GET();
        rptaSrv(httpCode1);
        delay(500);
        http.begin("192.168.4.1", 80, "/led1"); //HTTP
        int httpCode2 = http.GET();
        rptaSrv(httpCode2);
        delay(500);
    }
}
