#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"      
#define WLAN_SSID       "i net fiber"
#define WLAN_PASS       "903042394"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME  "rahul4271"
#define AIO_KEY       "aio_AZEQ81MHwCG0YGsdhMbV24UMYnT1"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish LDR = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/intensity");
int ldr = 0,temp=0;  //Stores intensity value
void setup() {
  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  connect();
}
void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void loop() {
  temp=analogRead(A0);
  ldr=map(temp,0,1023,0,100);
  if(! mqtt.ping(3)) {
    if(! mqtt.connected())
      connect();
  }
  Serial.println((int)ldr); 
  delay(5000);
   
      if (! LDR.publish(ldr)) {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    }
    else {
      Serial.println(F("Sent!"));
    }
}
