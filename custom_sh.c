/*
Paul lalwor
Systems Integration assignment 1
Custom shell using tiny shell
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

int pwd_func(char **args);
int date_func(char **args);

int pwd_func(char **args)
{
        // If no argument is passed
        if(args[1] == NULL)
        {
                system("pwd");
        }

        else
        {
                system("pwd");
        }
        return 1;
}


int date_func(char **args)
{
        // Storing the current time as a variable
        time_t now = time(0);
        char buffer[100];

        // Formatting the datetime string
        strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", localtime(&now));
        printf("%s\n",buffer);

        return 1;
}

