#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "relais.h"
#include "controlclient.h"
#include "switchwatcher.h"
#include "controlwebserver.h"

Relais relais0(D1);
Relais relais1(D2);
ControlClient controlClient0("192.168.0.2", 1234, "vorzimmer_decke", relais0);
ControlClient controlClient1("192.168.0.2", 1234, "wohnzimmer_decke", relais1);
SwitchWatcher watcher0(D5, relais0);
SwitchWatcher watcher1(D6, relais1);
ControlWebserver webserver;

void setup() {
  WiFi.begin("McDonalds Free WiFi 2.4GHz", "Passwort_123");
  
  relais0.begin();
  relais1.begin();

  controlClient0.begin();
  controlClient1.begin();

  watcher0.begin();
  watcher1.begin();

  webserver.addRelais0(relais0);
  webserver.addRelais1(relais1);

  webserver.begin();
}
 
void loop() {
    if(WiFi.status() == WL_CONNECTED) {
        controlClient0.handleClient();
        controlClient1.handleClient();
        webserver.handleClient();
    }
  
    watcher0.handle();
    watcher1.handle();
    
    delay(1);
}
