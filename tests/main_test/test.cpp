#include "collision_behaviour.hpp"
#include "sensor_thread.hpp"

#include <robotdriver/driver.h>
#include <librobot/rocket.h>

#include <unistd.h>
#include <iostream>
#include <chrono>


int time_elapsed_millis(const std::chrono::system_clock::time_point& beg)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - beg).count();
}

void move_and_act()
{
    //TODO : add vincent code
    while(true)
    {
        std::cout<<"We are moving in vincent code but not yet there"<<std::endl;
        sleep(1);
    }
}

void clean()
{
    //TODO : stop
}

int main()
{
    clean(); //dirty : when this program is terminated, it should be called again in order to clean state

    Sensor_Thread collision_detection(std::bind(&Collision_Behaviour::react_on_obstacle, std::placeholders::_1, std::placeholders::_2));
    initRoof();

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

    stopRobot();
    if(exit_fast)
        return 0;

    std::cout<<"[+] Launching final action"<<std::endl;
    launchRocket();

    sleep(4);

    std::cout<<"[-] Time elapsed, ending"<<std::endl;
    return 0;
}
