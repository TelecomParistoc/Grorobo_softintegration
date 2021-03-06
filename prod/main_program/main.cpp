#include "collision_behaviour.hpp"
#include "sensor_thread.hpp"
#include "robottop.h"

#include <robotdriver/driver.h>
#include <librobot/robot.h>

#include <unistd.h>
#include <iostream>
#include <chrono>


int time_elapsed_millis(const std::chrono::system_clock::time_point& beg)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - beg).count();
}

int main()
{
    Sensor_Thread collision_detection(std::bind(&Collision_Behaviour::react_on_obstacle, std::placeholders::_1, std::placeholders::_2));

    setYellowLed(true);
    while(getStartJack())
        usleep(10000);

    std::cout<<"[+] Jack inserted"<<std::endl;
    setGreenLed(true);

    while(!getStartJack())
        usleep(10000);

    std::cout<<"[+] Jack pulled, starting"<<std::endl;
    setYellowLed(false);
    std::thread actions_move_thread(std::bind(&moveAndAct));

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    #ifdef BIG
    bool exit_fast = false;
    #endif
    while(true)
    {
        int elapsed = time_elapsed_millis(start);
        if(elapsed > MAX_GAME_LENGTH_MILLIS) // game is over
            break;

        if(!getStartJack())
        {
            std::cout<<"[-] Jack pushed, ending"<<std::endl;
            #ifdef BIG
            exit_fast = true;
            #endif
            break;
        }

        usleep(10000);
    }

    setRedLed(true);
    setYellowLed(true);
    setGreenLed(true);
    
    #ifdef BIG
    if(!exit_fast)
    {
        std::cout<<"[+] Launching final action"<<std::endl;
        finishAction();
    }
    #endif

    sleep(4);

    setRedLed(false);
    setYellowLed(false);
    setGreenLed(false);

    std::cout<<"[-] Time elapsed, ending"<<std::endl;
    stopGame();

    collision_detection.stop();

    return 0;
}
