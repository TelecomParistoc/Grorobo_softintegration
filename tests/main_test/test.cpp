#include "sensor_thread.hpp"
#include "fToit.h"

#include <unistd.h>
#include <iostream>
#include <chrono>


std::chrono::high_resolution_clock clock;

int time_elapsed_millis(const std::chrono::time_point<clock>& beg)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - beg).count();
}

int main()
{
    Sensor_Thread collision_detection(callback_obstacle, callback_sensors);
    init_roof();

    setYellowLed(true);
    while(!getStartJack())
        usleep(10000);

    std::cout<<"[+] Jack inserted"<<std::endl;
    setGreenLed(true);

    while(getStartJack())
        usleep(10000);

    std::cout<<"[+] Jack pulled, starting"<<std::endl;
    setYellowLed(false);

    std::chrono::time_point<clock> start;
    bool final_action_launched = false;
    while(true)
    {
        std::cout<<time_elapsed_millis(start)<<std::endl;
        int elapsed = time_elapsed_millis(start);
        if(elapsed > MAX_GAME_LENGTH_MILLIS) // game is over
            break;
        else if(elapsed > FINAL_ACTION_DELAY_MILLIS && !final_action_launched)
        {
            final_action_launched = true;
            //TODO : ax12-move
        }

        if(getStartJack())
        {
            std::cout<<"[-] Jack pushed, ending"<<std::endl;
            stop();
            return 0;
        }

        usleep(10000);
    }

    std::cout<<"[-] Time elapsed, ending"<<std::endl;
    stop();

    return 0;
}
