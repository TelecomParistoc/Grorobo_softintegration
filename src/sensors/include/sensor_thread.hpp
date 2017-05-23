#include <functional>
#include <vector>


#define BLACK_SENSORS_SIDE 0
#define BLACK_SENSOR_MIDDLE 2
#define YELLOW_SENSORS_SIDE 6
#define YELLOW_SENSOR_MIDDLE 5


class Sensor_Thread
{
    public:
        void init();

        //sets the function that is called when an obstacle sensor state switches
        void set_callback_obstacle(const std::function<void(int, bool)>& callback);
        //returns true if desired sensor is triggered, false otherwise
        bool read_obstacle_sensor(int type) const;
        //returns mapping of current activated sensors
        const std::vector<bool>& get_active_sensors();

    private:
        std::function<void(int, bool)> _obstacle_callback; //int parameter is type of sensor that switched, bool is if sensor is now triggered
        std::vector<bool> _obstacle_sensors_activated;
};
