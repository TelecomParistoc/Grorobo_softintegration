#ifndef SINGLETON_HPP
#define SINGLETON_HPP


template <typename T>
class Singleton
{
    public:
        static T& instance();

        Singleton(const Singleton<T>& arg) = delete;
    	Singleton(const Singleton<T>&& arg) = delete;
    	Singleton& operator=(const Singleton<T>& arg) = delete;
    	Singleton& operator=(const Singleton<T>&& arg) = delete;

    protected:
        Singleton();

    private:
        static T _instance;
};

template <typename T>
T Singleton<T>::_instance;

template <typename T>
T& Singleton<T>::instance()
{
	return _instance;
}

template <typename T>
Singleton<T>::Singleton()
{}


#endif
