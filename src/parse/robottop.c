#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <librobot/robot.h>
#include "include/robottop.h"


void moveAndAct()
{
    // init grobot motors
    initMoteurs();

    //ouvre le fichier et la tache sera stocké dans action
    int c;
    FILE* file = NULL;
    file = fopen("command_grobot_homolo.txt", "r");
    c = fgetc(file);
    while(c != EOF)
    {
        if(readAndCall(file, c))
	    return;
        c = fgetc(file);
    }
    fclose(file);
    return;
}
