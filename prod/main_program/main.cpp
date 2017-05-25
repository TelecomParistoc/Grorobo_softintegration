#include "sensor_thread.hpp"
#include "roof.hpp"


int main()
{
    Sensor_Thread test(callback1, callback2);



    while(true)
        usleep(10000);
}
