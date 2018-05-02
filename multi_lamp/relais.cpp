#include "relais.h"

#include <Arduino.h>

Relais::Relais(const int pin) :
    m_pin(pin)
{
}

void Relais::begin()
{
	pinMode(m_pin, OUTPUT);
	on();
}

void Relais::on()
{
	digitalWrite(m_pin, HIGH);
}

void Relais::off()
{
	digitalWrite(m_pin, LOW);
}

void Relais::toggle()
{
	digitalWrite(m_pin, status() ? LOW : HIGH);
}

bool Relais::status() const
{
	return digitalRead(m_pin) == HIGH;
}
