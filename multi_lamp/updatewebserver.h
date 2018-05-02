#pragma once

#include <ESP8266WebServer.h>

class UpdateWebserver : public ESP8266WebServer
{
    static const char* m_content;

public:
    explicit UpdateWebserver(IPAddress addr, int port = 80);
    explicit UpdateWebserver(int port = 80);
    virtual ~UpdateWebserver();

private:
    void init();
};