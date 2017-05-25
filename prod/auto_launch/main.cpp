// Main program used by both robots
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "/home/pi/main_config.h" //TODO : change to a relative path


int main()
{
    std::string test_or_game_pin;
    std::ostringstream oss;
    oss<<TEST_OR_GAME_PIN;
    test_or_game_pin = oss.str();

    system(("gpio mode "+test_or_game_pin+" up").c_str());
    int current_state = 0;
    while(true)
    {
        system(("gpio read "+test_or_game_pin+" > tmp").c_str());
        std::ifstream ifs("tmp", std::ios::in);
        ifs>>current_state;
	
	std::cout<<"Read "<<current_state<<" state"<<std::endl;
        if(current_state) // game mode => main program used
        {
            int pid = fork();
            if(pid < 0)
            {
                std::cerr<<"[-] Error during fork"<<std::endl;
                exit(-1);
            }
            else if(pid)
	    {
		int status = 0;
                wait(&status);
		std::cout<<"[+] Program exited with status "<<status<<std::endl;
	    }
            else
	    {
		std::cout<<"[+] Executing subprogram "<<MAIN_PROGRAM<<std::endl;
                if(execl(MAIN_PROGRAM, MAIN_PROGRAM) < 0)
                {
                    std::cerr<<"[-] Error during execl"<<std::endl;
                    exit(-1);
                }
	    }
        }

        usleep(100000);
    }
}
