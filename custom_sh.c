/*
Paul lalwor
Systems Integration assignment 1
custom shell 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

int pwd_func(char **args);
int date_func(char **args);
int ifc_func(char **args);



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

int ifc_func(char **args)
{
        FILE *fp;
        char ifc_data[1024];
        char command[100];

        // If no argument is passed
        if(args[1] == NULL)
        {
                system("ifconfig eth0");
        }

        // If the user passes in an interface, call the ifconfig command for that interface
        else if(args[2] == NULL)
        {
                // Combine the interface name with the ifconfig command and store the result in the variable
                sprintf(command, "/sbin/ifconfig %s", args[1]);

                // Run the command
                fp = popen(command, "r");

                // Print the results
                while (fgets(ifc_data, sizeof(ifc_data), fp) != NULL)
                {
                        printf("%s", ifc_data);
                }

                pclose(fp);
        }

        // If the user passes an interface AND an IP address, validate the IP address and assign
        // it to the interface
        else if(args[3] == NULL)
        {
                int result;

                //
                result = ip_validator(args[2]);
                if(result == 1)
                {
                        // Combine the IP address and interface name with the ifconfig command and store the result in the variable
                        sprintf(command, "sudo /sbin/ifconfig %s %s", args[1], args[2]);

                        // Run the command
                        fp = popen(command, "r");

                        // Print the results
                        while (fgets(ifc_data, sizeof(ifc_data), fp) != NULL)
                        {
                                printf("%s", ifc_data);
                        }
                }

                else
                {
                        fprintf(stderr, "tsh: invalid argument to \"ifc\"\n");

                }

                pclose(fp);
        }

        else
        {
                fprintf(stderr, "tsh: invalid argument to \"ifc\"\n");
        }
        return 1;
}


