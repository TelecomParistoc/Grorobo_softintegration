#ifndef BOARD_SINGLETON_HPP
#define BOARD_SINGLETON_HPP


class Board_Singleton : public Singleton<Board_Singleton>
{
    public:
        friend class Singleton<Board_Singleton>;

        void add_digital_input_pin(int pin, bool pull_up);
        void add_digital_output_pin(int pin);

    private:
        Board_Singleton();

        void init();
};


#endif
