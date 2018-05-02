#include "switchwatcher.h"

#include <Arduino.h>

#include "relais.h"

SwitchWatcher::SwitchWatcher(const int pin, Relais &relais) :
    m_pin(pin),
    m_relais(relais)
{
}

void SwitchWatcher::begin()
{
    pinMode(m_pin, INPUT_PULLUP);
    m_lastState = status();
    m_countdown = 0;
}

void SwitchWatcher::handle()
{
    if(status() != m_lastState)
    {
        if(++m_countdown >= 10)
        {
            m_relais.toggle();
            m_lastState = !m_lastState;
            m_countdown = 0;
        }
    }
    else
        m_countdown = 0;
}

bool SwitchWatcher::status() const
{
    return digitalRead(m_pin) == HIGH;
}
