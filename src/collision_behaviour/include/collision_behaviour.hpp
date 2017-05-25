#ifndef COLLISION_BEHAVIOUR_HPP
#define COLLISION_BEHAVIOUR_HPP


class Collision_Behaviour
{
    public:
        static void react_on_obstacle(bool forward_sensors_activated, bool backward_sensors_activated);

        static void pause();
        static void unpause();

    private:
        static bool is_blocked;
};


#endif
