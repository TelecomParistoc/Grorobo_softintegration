#include "sensor_thread.hpp"

#include <iostream>


void print_obstacle(int desc)
{
    std::cout<<"Sensor "<<Sensor_Thread::sensor_name(desc)<<" detected obstacle"<<std::endl;
}

void print_sensor_switch(int desc, bool state)
{
    if(state)
        std::cout<<"Sensor "<<Sensor_Thread::sensor_name(desc)<<" switched to high state"<<std::endl;
    else
        std::cout<<"Sensor "<<Sensor_Thread::sensor_name(desc)<<" switched to low state"<<std::endl;
}

int main()
{
    Sensor_Thread test(std::bind(print_obstacle, std::placeholders::_1), std::bind(print_sensor_switch, std::placeholders::_1, std::placeholders::_2));

    while(true)
        usleep(10000);
}
