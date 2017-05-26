#include "include/collision_behaviour.hpp"
#include <robotdriver/driver.h>


bool Collision_Behaviour::is_blocked = false;

void Collision_Behaviour::react_on_obstacle(bool backward_sensors_activated, bool forward_sensors_activated)
{
    int direction = getDirection();

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
