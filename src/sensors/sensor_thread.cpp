#include "include/sensor_thread.hpp"

#include <wiringPi.h>
#include <unistd.h>
#include <cassert>


std::map<int, std::string> Sensor_Thread::_sensor_names = {{BLACK_SENSORS_SIDE, "BLACK_SENSORS_SIDE"},
														  {BLACK_SENSOR_MIDDLE, "BLACK_SENSOR_MIDDLE"},
														  {YELLOW_SENSORS_SIDE, "YELLOW_SENSORS_SIDE"},
														  {YELLOW_SENSOR_MIDDLE, "YELLOW_SENSOR_MIDDLE"}};

std::map<int, int> Sensor_Thread::_sensor_descriptions_to_ids = {{BLACK_SENSORS_SIDE, 0},
														  		{BLACK_SENSOR_MIDDLE, 1},
														  		{YELLOW_SENSORS_SIDE, 2},
														  		{YELLOW_SENSOR_MIDDLE, 3}};

Sensor_Thread::Sensor_Thread(const std::function<void(int)>& obstacle_callback, const std::function<void(int, bool)>& sensor_callback, bool _init) :
	_sensor_callback(sensor_callback),
	_obstacle_callback(obstacle_callback)
{
	if(_init)
		init();
}

void Sensor_Thread::init()
{
	wiringPiSetup();

	_pin_id[_sensor_descriptions_to_ids[BLACK_SENSORS_SIDE]] = BLACK_SENSORS_SIDE;
	_pin_id[_sensor_descriptions_to_ids[BLACK_SENSOR_MIDDLE]] = BLACK_SENSOR_MIDDLE;
	_pin_id[_sensor_descriptions_to_ids[YELLOW_SENSORS_SIDE]] = YELLOW_SENSORS_SIDE;
	_pin_id[_sensor_descriptions_to_ids[YELLOW_SENSOR_MIDDLE]] = YELLOW_SENSOR_MIDDLE;

	_pull_up[_sensor_descriptions_to_ids[BLACK_SENSORS_SIDE]] = false;
	_pull_up[_sensor_descriptions_to_ids[BLACK_SENSOR_MIDDLE]] = false;
	_pull_up[_sensor_descriptions_to_ids[YELLOW_SENSORS_SIDE]] = true;
	_pull_up[_sensor_descriptions_to_ids[YELLOW_SENSOR_MIDDLE]] = true;

	for(int i=0; i<N_Sensors; i++)
	{
		pinMode(_pin_id[i], INPUT);
		_state[i] = false;
		if(!_pull_up[i])
		{
			pullUpDnControl(_pin_id[i], PUD_DOWN);
			_cur_scale_pin[i] = 0;
		}
		else
		{
			pullUpDnControl(_pin_id[i], PUD_UP);
			_cur_scale_pin[i] = To_Keep_For_Majority;
		}
	}

	start();
}

void Sensor_Thread::start()
{
	stop();
	_acquire_and_react = std::thread(std::bind(&Sensor_Thread::run, this));
}

void Sensor_Thread::stop()
{
	_stop = true;
	if(_acquire_and_react.joinable())
		_acquire_and_react.join();
}

void Sensor_Thread::set_obstacle_callback(const std::function<void(int)>& callback)
{
	_functions_mutex.lock();
	_obstacle_callback = callback;
	_functions_mutex.unlock();
}

void Sensor_Thread::set_sensor_callback(const std::function<void(int, bool)>& callback)
{
	_functions_mutex.lock();
	_sensor_callback = callback;
	_functions_mutex.unlock();
}

bool Sensor_Thread::read_obstacle_sensor_from_id(int index) const
{
	assert(index>=0 && index<N_Sensors);
	return _state.at(index);
}

bool Sensor_Thread::read_obstacle_sensor_from_description(int desc) const
{
	assert(_sensor_descriptions_to_ids.count(desc));
	return _state.at(_sensor_descriptions_to_ids.at(desc));
}

const std::vector<bool>& Sensor_Thread::get_active_sensors()
{
	_update_mutex.lock();
	_tmp_sensors = _obstacle_sensors_activated;
	_update_mutex.unlock();
	return _tmp_sensors;
}

void Sensor_Thread::run()
{
	_stop = false;
	while(!_stop)
	{
		std::vector<int> index;
		bool recompute = false;

		for(int i=0; i<N_Sensors; i++)
		{
			if(digitalRead(_pin_id[i]))
			{
			 	if(_cur_scale_pin[i]+1<To_Keep_For_Majority)
					_cur_scale_pin[i]++;
			}
			else
				if(_cur_scale_pin[i]>0)
					_cur_scale_pin[i]--;

			bool next_state = (!_pull_up[i] && _cur_scale_pin[i]*4>=3*To_Keep_For_Majority) || (_pull_up[i] && _cur_scale_pin[i]*4<To_Keep_For_Majority); //threshold effect
			if(next_state != _state[i])
			{
				recompute = true;
				index.push_back(i);
				_state[i]= next_state;
			}
		}

		if(recompute)
		{
			_update_mutex.lock();
			_obstacle_sensors_activated.clear();
			for(int i=0; i<N_Sensors; i++)
				if(_state[i])
					_obstacle_sensors_activated.push_back(_pin_id[i]);
			_update_mutex.unlock();

			_functions_mutex.lock();
			for(int i : index)
			{
				if(_sensor_callback)
					_sensor_callback(_pin_id[i], _state[i]);
				if(_state[i] && _obstacle_callback)
					_obstacle_callback(_pin_id[i]);
			}
			_functions_mutex.unlock();
		}
		usleep(1000000);
	}
}

const std::map<int, std::string>& Sensor_Thread::sensor_names()
{return _sensor_names;}

const std::string& Sensor_Thread::sensor_name(int desc)
{return _sensor_names[desc];}
