#include "collision_behaviour.hpp"
#include "sensor_thread.hpp"

#include <robotdriver/driver.h>

#include <unistd.h>
#include <iostream>
#include <chrono>


std::chrono::high_resolution_clock clock;

int time_elapsed_millis(const std::chrono::time_point<clock>& beg)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - beg).count();
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

    Sensor_Thread collision_detection(std::bind(&Collision_Behaviour::react_on_obstacle));
    init_roof();
    std::thread actions_move_thread(move_and_act);

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
            std::cout<<"[+] Launching final action"<<std::endl;
            final_action_launched = true;
            //TODO : ax12-move
        }

        if(getStartJack())
        {
            std::cout<<"[-] Jack pushed, ending"<<std::endl;
            std::terminate();
        }

        usleep(10000);
    }

    std::cout<<"[-] Time elapsed, ending"<<std::endl;
    std::terminate();
}
