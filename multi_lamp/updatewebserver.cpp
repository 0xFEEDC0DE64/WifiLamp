#include "updatewebserver.h"

const char* UpdateWebserver::m_content =
    "<!doctype html>"
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

UpdateWebserver::UpdateWebserver(IPAddress addr, int port) :
    ESP8266WebServer(addr, port)
{
    init();
}

UpdateWebserver::UpdateWebserver(int port) :
    ESP8266WebServer(port)
{
    init();
}

UpdateWebserver::~UpdateWebserver()
{
}

void UpdateWebserver::init()
{
    on("/update", HTTP_GET, [this]() {
        sendHeader("Connection", "close");
        send(200, "text/html", m_content);
    });

    on("/update", HTTP_POST, [this]() {
        sendHeader("Connection", "close");
        send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, [this]() {
        HTTPUpload& upload = this->upload();
        if (upload.status == UPLOAD_FILE_START) {
            //Serial.setDebugOutput(true);
            //Serial.printf("Update: %s\n", upload.filename.c_str());
      
            uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
            if (!Update.begin(maxSketchSpace)) { //start with max available size
                //Update.printError(Serial);
            }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                //Update.printError(Serial);
            }
      } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) { //true to set the size to the current progress
                //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            } else {
                //Update.printError(Serial);
            }
  
            //Serial.setDebugOutput(false);
        }
        yield();
    });
}
