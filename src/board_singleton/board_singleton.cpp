#include "include/board_singleton.hpp"

#include <wiringPi.h>


void Board_Singleton::add_digital_input_pin(int pin, bool pull_up)
{
    pinMode(pin, INPUT);
    if(pull_up)
        pullUpDnControl(pin, PUD_UP);
    else
        pullUpDnControl(pin, PUD_DOWN);
}

void Board_Singleton::add_digital_output_pin(int pin)
{
    pinMode(pin, OUTPUT);
}

bool Board_Singleton::digital_read(int pin)
{
    return digitalRead(pin);
}

void Board_Singleton::digital_write(int pin, bool val)
{
    digitalWrite(pin, val);
}


Board_Singleton::Board_Singleton()
{
    init();
}

void Board_Singleton::init()
{
    wiringPiSetup();
}
