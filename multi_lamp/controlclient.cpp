#include "controlclient.h"

#include "relais.h"

ControlClient::ControlClient(const char* host, const int port, const char* name, Relais &relais) :
    m_host(host),
    m_port(port),
    m_name(name),
    m_relais(relais)
{
}

void ControlClient::begin()
{
}

void ControlClient::handleClient()
{
    if(!m_client.connected()) {
        if (m_client.connect(m_host, m_port)) {
            m_client.println(m_name);
            sendStatus();
        }
    }

    if(m_client.connected()) {
        while(m_client.available()) {
            switch(m_client.read()) {
                case '1': m_relais.on(); break;
                case '0': m_relais.off(); break;
                case 't': m_relais.toggle(); break;
                case 's': sendStatus(); break;
                case 'r': m_client.println("rebooting"); ESP.restart(); break;
            }
        }
    }
}

void ControlClient::sendStatus()
{
    m_client.println(m_relais.status() ? "on" : "off");
}
