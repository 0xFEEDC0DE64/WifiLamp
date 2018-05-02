#pragma once

class Relais
{
public:
	explicit Relais(const int pin);

	void begin();

	void on();
	void off();
	void toggle();
	bool status() const;

private:
	const int m_pin;
};
