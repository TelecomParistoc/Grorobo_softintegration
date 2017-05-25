#include "board_singleton/include/board_singleton.hpp"
#include "include/sensor_thread.hpp"

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

Sensor_Thread::Sensor_Thread(const std::function<void(bool, bool)>& event_callback, const std::function<void(int)>& obstacle_callback, const std::function<void(int, bool)>& sensor_callback, bool _init) :
	_sensor_callback(sensor_callback),
	_obstacle_callback(obstacle_callback),
	_event_callback(event_callback)
{
	if(_init)
		init();
}

void Sensor_Thread::init()
{
	_pin_id[_sensor_descriptions_to_ids[BLACK_SENSORS_SIDE]] = BLACK_SENSORS_SIDE;
	_pin_id[_sensor_descriptions_to_ids[BLACK_SENSOR_MIDDLE]] = BLACK_SENSOR_MIDDLE;
	_pin_id[_sensor_descriptions_to_ids[YELLOW_SENSORS_SIDE]] = YELLOW_SENSORS_SIDE;
	_pin_id[_sensor_descriptions_to_ids[YELLOW_SENSOR_MIDDLE]] = YELLOW_SENSOR_MIDDLE;

	_pull_up[_sensor_descriptions_to_ids[BLACK_SENSORS_SIDE]] = false;
	_pull_up[_sensor_descriptions_to_ids[BLACK_SENSOR_MIDDLE]] = false;
	_pull_up[_sensor_descriptions_to_ids[YELLOW_SENSORS_SIDE]] = true;
	_pull_up[_sensor_descriptions_to_ids[YELLOW_SENSOR_MIDDLE]] = true;

	_forward[_sensor_descriptions_to_ids[BLACK_SENSORS_SIDE]] = true;
	_forward[_sensor_descriptions_to_ids[BLACK_SENSOR_MIDDLE]] = true;
	_forward[_sensor_descriptions_to_ids[YELLOW_SENSORS_SIDE]] = false;
	_forward[_sensor_descriptions_to_ids[YELLOW_SENSOR_MIDDLE]] = false;


	Board_Singleton& board = Board_Singleton::instance();
	for(int i=0; i<N_Sensors; i++)
	{
		_state[i] = false;
		if(!_pull_up[i])
		{
			board.add_digital_input_pin(_pin_id[i], false);
			_cur_scale_pin[i] = 0;
		}
		else
		{
			board.add_digital_input_pin(_pin_id[i], true);
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

void Sensor_Thread::set_simplest_event_callback(const std::function<void(bool, bool)>& callback)
{
	_functions_mutex.lock();
	_event_callback = callback;
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

bool Sensor_Thread::is_blocked()
{
	_update_mutex.lock();
	unsigned int ret = _obstacle_sensors_activated.size();
	_update_mutex.unlock();
	return (bool)ret;
}

void Sensor_Thread::run()
{
	Board_Singleton& board = Board_Singleton::instance();

	_stop = false;
	while(!_stop)
	{
		std::vector<int> index;
		bool recompute = false;

		for(int i=0; i<N_Sensors; i++)
		{
			if(board.digital_read(_pin_id[i]))
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

			bool forward_sensor = false, backward_sensor = false;
			_functions_mutex.lock();
			for(int i : index)
			{
				if(_sensor_callback)
					_sensor_callback(_pin_id[i], _state[i]);
				if(_state[i] && _obstacle_callback)
					_obstacle_callback(_pin_id[i]);
				if(_state[i] && _forward[i])
					forward_sensor = true;
				else if(_state[i] && !_forward[i])
					backward_sensor = true;
			}
			_event_callback(forward_sensor, backward_sensor);
			_functions_mutex.unlock();
		}
	}
}

const std::map<int, std::string>& Sensor_Thread::sensor_names()
{return _sensor_names;}

const std::string& Sensor_Thread::sensor_name(int desc)
{return _sensor_names[desc];}
