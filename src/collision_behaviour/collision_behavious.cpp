#include "include/collision_behaviour.hpp"
#include <robotdriver/driver.h>


bool collision_behaviour::is_blocked = false;

void Collision_Behaviour::react_on_obstacle(bool forward_sensors_activated, bool backward_sensors_activated)
{
    bool pause_engine = false;

    int direction = getDirection();
    if(direction == DIR_NONE)
        return;

    if(is_blocked)
    {
        if((!forward_sensors_activated || direction != DIR_FORWARD) && (!backward_sensors_activated || direction != DIR_BACKWARD))
            unpause();
    }
    else
        if((forward_sensors_activated && direction == DIR_FORWARD) || (backward_sensors_activated && direction == DIR_BACKWARD))
            pause();
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
