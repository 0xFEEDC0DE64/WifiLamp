#include "controlwebserver.h"

#include "relais.h"

const char* ControlWebserver::m_content =
    "<!doctype html>"
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

ControlWebserver::ControlWebserver(IPAddress addr, int port) :
    UpdateWebserver(addr, port)
{
    init();
}

ControlWebserver::ControlWebserver(int port) :
    UpdateWebserver(port)
{
    init();
}

ControlWebserver::~ControlWebserver()
{
}

void ControlWebserver::addRelais0(Relais &relais)
{
    addRelais("/on0", "/off0", "/toggle0", "/status0", relais);
}

void ControlWebserver::addRelais1(Relais &relais)
{
    addRelais("/on1", "/off1", "/toggle1", "/status1", relais);
}

void ControlWebserver::addRelais(const char *onUrl, const char *offUrl, const char *toggleUrl, const char *statusUrl, Relais &relais)
{
    on(onUrl, HTTP_GET, [this, &relais]() {
        relais.on();
        
        sendHeader("Connection", "close");
        send(200, "text/plain", "success");
    });
    
    on(offUrl, HTTP_GET, [this, &relais]() {
        relais.off();
        
        sendHeader("Connection", "close");
        send(200, "text/plain", "success");
    });
    
    on(toggleUrl, HTTP_GET, [this, &relais]() {
        relais.toggle();
        
        sendHeader("Connection", "close");
        send(200, "text/plain", "success");
    });
    
    on(statusUrl, HTTP_GET, [this, &relais]() {
        sendHeader("Connection", "close");
        send(200, "text/plain", relais.status() ? "on" : "off");
    });
}

void ControlWebserver::init()
{
    on("/", HTTP_GET, [this]() {
        sendHeader("Connection", "close");
        send(200, "text/html", m_content);
    });
}
