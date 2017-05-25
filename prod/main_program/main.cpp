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

/*void move_and_act()
{
    //TODO : add vincent code
    while(true)
    {
        std::cout<<"We are moving in vincent code but not yet there"<<std::endl;
        sleep(1);
    }
}*/

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
    std::thread actions_move_thread(move_and_act);

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    bool exit_fast = false;
    while(true)
    {
        int elapsed = time_elapsed_millis(start);
        if(elapsed > MAX_GAME_LENGTH_MILLIS) // game is over
            break;

        if(!getStartJack())
        {
            std::cout<<"[-] Jack pushed, ending"<<std::endl;
            exit_fast = true;
            break;
        }

        usleep(10000);
    }

    if(!exit_fast)
    {
        std::cout<<"[+] Launching final action"<<std::endl;
        finishAction();
	    sleep(4);
    }

    std::cout<<"[-] Time elapsed, ending"<<std::endl;
    stopGame();

    collision_detection.stop();

    return 0;
}
