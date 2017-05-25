#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <librobot/robot.h>

int main()
{
    // init grobot motors
    initMoteurs();

    //ouvre le fichier et la tache sera stocké dans action
    char c;
    FILE* file = NULL;
    file = fopen("command_grobot_homolo.txt", "r");
    c = fgetc(file);
    while(c != EOF && c != '\0')
    {
        if(readAndCall(file, c))
	    return 0;
        c = fgetc(file);
    }
    fclose(file);
    return 0;
}
