#pragma once

class Relais;

class SwitchWatcher
{
public:
    explicit SwitchWatcher(const int pin, Relais &relais);

    void begin();
    void handle();

    bool status() const;

private:
    const int m_pin;
    Relais &m_relais;

    bool m_lastState;
    int m_countdown;
};
