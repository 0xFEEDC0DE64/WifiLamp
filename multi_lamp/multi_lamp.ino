#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "McDonalds Free WiFi 2.4GHz";
const char* password = "Passwort_123";
const char* host = "192.168.0.2";

const char* success = "success";

const char* indexContent = "<!doctype html>"
                           "<html lang=\"de-AT\">"
                               "<head>"
                                   "<meta charset=\"utf-8\" />"
                                   "<meta http-equiv=\"x-ua-compatible\" content=\"ie=edge\" />"
                                   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
                                   
                                   "<title>Relais Control</title>"
                                   
                                   "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\" />"
                                   "<style>.hidden { display: none; }</style>"
                               "</head>"
                               "<body>"
                                   "<!--[if lte IE 9]>"
                                       "<p class=\"browserupgrade\">You are using an <strong>outdated</strong> browser. Please <a href=\"https://browsehappy.com/\">upgrade your browser</a> to improve your experience and security.</p>"
                                   "<![endif]-->"
                                   
                                   "<div class=\"container\">"
                                       "<h1>Relais Control</h1>"
                                       
                                       "<switch onUrl=\"on0\" offUrl=\"off0\" statusUrl=\"status0\" />"
                                       "<switch onUrl=\"on1\" offUrl=\"off1\" statusUrl=\"status1\" />"
                                   "</div>"
                                   
                                   "<script src=\"https://code.jquery.com/jquery-3.3.1.min.js\" integrity=\"sha384-tsQFqpEReu7ZLhBV2VZlAu7zcOV+rXbYlF2cqB8txI/8aZajjp4Bqd+V6D5IgvKT\" crossorigin=\"anonymous\"></script>"
                                   "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js\" integrity=\"sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl\" crossorigin=\"anonymous\"></script>"
                                   "<script src=\"https://brunner.ninja/relais.js\"></script>"
                               "</body>"
                           "</html>";

const char* updateContent = "<!doctype html>"
                            "<html lang=\"de-AT\">"
                                "<head>"
                                    "<meta charset=\"utf-8\" />"
                                    "<meta http-equiv=\"x-ua-compatible\" content=\"ie=edge\" />"
                                    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
                                    
                                    "<title>Update</title>"
                                    
                                    "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\" />"
                                    "<style>.hidden { display: none; }</style>"
                                "</head>"
                                "<body>"
                                    "<!--[if lte IE 9]>"
                                        "<p class=\"browserupgrade\">You are using an <strong>outdated</strong> browser. Please <a href=\"https://browsehappy.com/\">upgrade your browser</a> to improve your experience and security.</p>"
                                    "<![endif]-->"
                                    
                                    "<div class=\"container\">"
                                        "<h1>Update</h1>"
                                        
                                        "<form method=\"POST\" action=\"/update\" enctype=\"multipart/form-data\">"
                                            "<input type=\"file\" name=\"update\" />"
                                            "<button type=\"submit\">Install</button>"
                                        "</form>"
                                    "</div>"
                                    
                                    "<script src=\"https://code.jquery.com/jquery-3.3.1.min.js\" integrity=\"sha384-tsQFqpEReu7ZLhBV2VZlAu7zcOV+rXbYlF2cqB8txI/8aZajjp4Bqd+V6D5IgvKT\" crossorigin=\"anonymous\"></script>"
                                    "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js\" integrity=\"sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl\" crossorigin=\"anonymous\"></script>"
                                "</body>"
                            "</html>";

class ControlClient {
public:
  explicit ControlClient(const char *name, const int pin) :
    m_name(name),
    m_pin(pin)
  {
  }

  void begin() {
    Serial.println(m_pin);
    Serial.println("begin()");
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, HIGH);
  }

  void handleClient() {
    if(!m_client.connected()) {
      Serial.println("Connecting to server...");
      
      if (m_client.connect(host, 1234)) {
        Serial.println("Connected to server!");

        m_client.println(m_name);
        sendStatus();
      } else {
        Serial.println("Could not connect to server!");
      }
    }

    if(m_client.connected()) {
      while(m_client.available()) {
        char c(m_client.read());
        Serial.println(c);
        switch(c) {
          case '1': on(); break;
          case '0': off(); break;
          case 't': toggle(); break;
          case 's': sendStatus(); break;
          default: Serial.print("Unknown command: "); Serial.println(c);
        }
      }
    }
  }

  void on() {
    Serial.println("on()");
    digitalWrite(m_pin, HIGH);
    sendStatus();
  }

  void off() {
    Serial.println("off()");
    digitalWrite(m_pin, LOW);
    sendStatus();
  }

  void toggle() {
    Serial.println("toggle()");
    if(status()) {
      off();
    } else {
      on();
    }
  }

  bool status() {
    return digitalRead(m_pin) == HIGH;
  }

  void sendStatus() {
    Serial.println("sendStatus()");
    m_client.println(status() ? "on" : "off");
  }

private:
  WiFiClient m_client;
  const char *m_name;
  const int m_pin;
};

ESP8266WebServer server(80);
ControlClient relais0Client("wohnzimmer_decke", D1);
ControlClient relais1Client("vorzimmer_decke", D2);

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);
  
  relais0Client.begin();
  relais1Client.begin();

  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", indexContent);
  });
  
  server.on("/update", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", updateContent);
  });
  
  server.on("/on0", HTTP_GET, []() {
    relais0Client.on();
    
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", success);
  });
  
  server.on("/off0", HTTP_GET, []() {
    relais0Client.off();
    
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", success);
  });
  
  server.on("/on1", HTTP_GET, []() {
    relais1Client.on();
    
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", success);
  });
  
  server.on("/off1", HTTP_GET, []() {
    relais1Client.off();
    
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", success);
  });
  
  server.on("/toggle0", HTTP_GET, []() {
    relais0Client.toggle();
    
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", success);
  });
  
  server.on("/toggle1", HTTP_GET, []() {
    relais1Client.toggle();
    
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", success);
  });
  
  server.on("/status0", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", relais0Client.status() ? "on" : "off");
  });
  
  server.on("/status1", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", relais1Client.status() ? "on" : "off");
  });
  
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.setDebugOutput(true);
      Serial.printf("Update: %s\n", upload.filename.c_str());

      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }

      Serial.setDebugOutput(false);
    }
    yield();
  });

  server.begin();
}
 
void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    relais0Client.handleClient();
    relais1Client.handleClient();
    server.handleClient();
  } else {
    Serial.println("No wifi");
    delay(500);
  }
  
  delay(1);
}
