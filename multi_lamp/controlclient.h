#pragma once

#include <WiFiClient.h>

class Relais;

class ControlClient {
public:
    explicit ControlClient(const char *host, const int port, const char *name, Relais &relais);

    void begin();
    void handleClient();

    void sendStatus();

private:
    WiFiClient m_client;
    const char *m_host;
    const int m_port;
    const char *m_name;
    Relais &m_relais;
};
