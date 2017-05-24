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

    private:
        Singleton();

        static T _instance;
};


#endif
