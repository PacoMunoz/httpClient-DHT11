
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <ezTime.h>

#include <WiFiUdp.h>
#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

// definicion del tipo de sensor DHT
#define DHTTYPE DHT11

// definir puerto de entrada del sensor de temperatura en la placa
const int DHTPin = 2;

// crear una instancia de la clase que maneja el sensor con los parametros deseados
DHT dht(DHTPin, DHTTYPE);

// definir parametros de la wifi local
const char* ssid = "MOVISTAR_23DF";
const char* password = "avDUjM5GSbuoWSHPKRzG";

String sensorPosition = "estudio";

// ubuntu
// String serverName = "http://192.168.1.35:8080/data";

// raspberry
String serverName = "http://192.168.1.68:8080/data";


void setup() {
  
  // inicializacion del puerto serie local
  Serial.begin(9600);

  // inicializacion de la instancia del sensor de temperatura
  dht.begin();

  // inicializacion de la conexion con la wifi 
  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); 
  }
  Serial.println("WIFI connected!");

  // Wait for ezTime to get its time synchronized
  waitForSync();
  // Set NTP polling interval to 60 seconds. Way too often, but good for demonstration purposes.
  setInterval(60);
      
}

void loop() {

  delay(5000);

  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      WiFiClient client;

      String serverPath = serverName;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());


     Serial.println("UTC: " + UTC.dateTime()); 
     
     // Definir el timezone para España
     Timezone Spain;
     Spain.setLocation("Europe/Madrid");

           Serial.println("Position: " + sensorPosition);
            
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      
      int httpResponseCode = http.POST("{\"date\":\" " + Spain.dateTime(ISO8601) 
                                      + "\",\"temperature\":\"" + String(dht.readTemperature())
                                      + "\",\"humidity\":\"" + String(dht.readHumidity())
                                      + "\",\"location\":\"" + sensorPosition +"\" }");
      
      
      /*if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }*/
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  
}
