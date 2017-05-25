#include "include/collision_behaviour.hpp"
#include <robotdriver/driver.h>

#include <iostream>

bool Collision_Behaviour::is_blocked = false;

void Collision_Behaviour::react_on_obstacle(bool forward_sensors_activated, bool backward_sensors_activated)
{
    int direction = getDirection();
    std::cout<<direction<<" "<<forward_sensors_activated<<" "<<backward_sensors_activated<<std::endl;

    if(direction == DIR_NONE)
        return;

    if(is_blocked)
    {
        if((!forward_sensors_activated || direction != DIR_FORWARD) && (!backward_sensors_activated || direction != DIR_BACKWARD))
        {
            unpause();
            setRedLed(false);
        }
    }
    else
        if((forward_sensors_activated && direction == DIR_FORWARD) || (backward_sensors_activated && direction == DIR_BACKWARD))
        {
            pause();
            setRedLed(true);
        }
}

void Collision_Behaviour::pause()
{
    is_blocked = true;
    stopRobot();
}

void Collision_Behaviour::unpause()
{
    is_blocked = false;
    resumeRobot();
}
