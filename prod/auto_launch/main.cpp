// Main program used by both robots

#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string>

#include "/home/pi/main_config.h" //TODO : change to a relative path


int main()
{
    std::string test_or_game_pin;
    std::ostringstream oss;
    oss<<TEST_OR_GAME_PIN;
    test_or_game_pin = oss.str();

    system("gpio mode "+test_or_game_pin+" up");
    int current_state = 0;
    while(true)
    {
        system("gpio read "+test_or_game_pin+" > tmp");
        std::ifstream ifs("tmp", std::ios::in);
        ifs>>current_state;

        if(current_state) // game mode => main program used
        {
            int pid = fork();
            if(pid < 0)
            {
                std::cerr<<"[-] Error during fork"<<std::endl;
                exit(-1);
            }
            else if(pid)
                wait(NULL);
            else
                if(execl(MAIN_PROGRAM, 0) < 0)
                {
                    std::cerr<<"[-] Error during execl"<<std::endl;
                    exit(-1);
                }
        }

        usleep(100000);
    }
}
