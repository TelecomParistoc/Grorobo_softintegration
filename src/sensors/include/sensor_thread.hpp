#include <functional>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>


#define BLACK_SENSORS_SIDE 0
#define BLACK_SENSOR_MIDDLE 2
#define YELLOW_SENSORS_SIDE 6
#define YELLOW_SENSOR_MIDDLE 5

#define N_Sensors 4
#define To_Keep_For_Majority 8


class Sensor_Thread
{
    public:
        Sensor_Thread(const std::function<void(int)>& obstacle_callback = std::function<void(int)>(), const std::function<void(int, bool)>& sensor_callback = std::function<void(int, bool)>(), bool init = true);

        void init();

        //starts thread, is automatically called in init (so in constructor if init=true)
        void start();
        //stops thread
        void stop();

        //sets the function that is called when an obstacle is detected
        void set_obstacle_callback(const std::function<void(int)>& callback);
        //sets the function that is called when a sensor state switches
        void set_sensor_callback(const std::function<void(int, bool)>& callback);
        //returns true if desired sensor is triggered, false otherwise, index is position in sensor array
        bool read_obstacle_sensor_from_id(int index) const;
        //returns true if desired sensor is triggered, false otherwise, index is sensor description (like BLACK_SENSOR_MIDDLE)
        bool read_obstacle_sensor_from_description(int desc) const;
        //returns mapping of current activated sensors
        const std::vector<bool>& get_active_sensors();

        static const std::map<int, std::string>& sensor_names();
        static const std::string& sensor_name(int desc);

    private:
        std::function<void(int, bool)> _sensor_callback; //int parameter is description (like BLACK_SENSOR_MIDDLE) of sensor that switched, bool is if sensor is now triggered
        std::function<void(int)> _obstacle_callback; //int parameter is description (like BLACK_SENSOR_MIDDLE) of sensor that detected switch
        std::vector<bool> _obstacle_sensors_activated;
        std::vector<bool> _tmp_sensors;
        std::array<int, N_Sensors> _cur_scale_pin;
        std::array<bool, N_Sensors> _pull_up;
        std::array<bool, N_Sensors> _state;
        std::array<int, N_Sensors> _pin_id;

        std::thread _acquire_and_react; //thread thats check sensor states and reacts in function of what change
        std::mutex _update_mutex;
        std::mutex _functions_mutex;
        bool _stop;

        void run();

        static std::map<int, std::string> _sensor_names;
        static std::map<int, int> _sensor_descriptions_to_ids;
};
