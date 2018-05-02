#pragma once

#include "updatewebserver.h"

class Relais;

class ControlWebserver : public UpdateWebserver
{
    static const char* m_content;

public:
    explicit ControlWebserver(IPAddress addr, int port = 80);
    explicit ControlWebserver(int port = 80);
    virtual ~ControlWebserver();

    void addRelais0(Relais &relais);
    void addRelais1(Relais &relais);
    void addRelais(const char *onUrl, const char *offUrl, const char *toggleUrl, const char *statusUrl, Relais &relais);

private:
    void init();
};
