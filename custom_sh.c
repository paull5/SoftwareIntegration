/*
	Paul lawlor c13764501
	SoftwareIntegration Assignment 1
	Custom shell
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <grp.h>
#include <sys/stat.h>

/*
	Declare functions for internal commands
*/

int pwd_func(char **args);
int date_func(char **args); 
int cd_func(char **args);
int help_func(char **args);
int logout_func(char **args);
int ifc_func(char **args);
int ud_func(char **args);

/*
	Global variables used for memory allocation in  read_line and split_line 
*/

#define cust_TOK_BUFSIZE 64
#define cust_TOK_DELIM " \t\r\n\a"
#define cust_RL_BUFSIZE 1024

/*
	Struct to hold  a list of commands.
*/
 
char *builtin_str[] = {
        "cd",
        "help",
        "logout",
        "pw",
        "ifc",
        "dt",
        "ud",
};


/*
	Functions for internal commands
*/

int (*builtin_func[]) (char **) = {
        &cd_func,
        &help_func,
        &logout_func,
        &pwd_func,
        &ifc_func,
        &date_func,
        &ud_func,
};


int num_builtins()
{
        return sizeof(builtin_str) / sizeof(char *);
}


/**
    pwd_func displays users current working directory
*/

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

/*
    date_func displays the current date formatted 
*/

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

/*
    ifc_func displays ifconfig information 
	When ifc is entered only eth0 information is displayed 
*/

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
		
        else
        {
                fprintf(stderr, "customsh: invalid argument to \"ifc\"\n");
        }
        return 1;
}


/*
	Print help function displays a lisy of allowed commands.  
*/

int help_func(char **args)
{
        int i;
        printf("Type help at the prompt to see a list of commands\n");
        printf("Type program names followed by any arguments, and press enter.\n");
        printf("These are the built in commands:\n");

        // Looping through and printing the list of pre-defined commands
        for (i = 0; i < num_builtins(); i++)
        {
                printf("  %s\n", builtin_str[i]);
        }
        return 1;
}

/**
    logout_func logs user out of shell 
 */
 
int logout_func(char **args)
{
        return 0;
}

/*
	cd_func changes directory when entered with appropriate arguments     
*/

int cd_func(char **args)
{
        // If no argument is passed print error message
        if (args[1] == NULL)
        {
                fprintf(stderr, "customsh: expected argument to \"cd\"\n");
        }
        else
        {
                if (chdir(args[1]) != 0)
                {
                        perror("customsh");
                }
        }
        return 1;
}

/*
    ud_func displays information about the user
*/

int ud_func(char **args)
{
        // Declaring the structures needed to get the group name inode
        struct group *g;
        struct stat s;

        // Getting the USER global variable
        char *p=getenv("USER");

        uid_t uid;
        gid_t gid;

        char home_dir[100];

        uid = getuid();
        gid = getgid();
        g = getgrgid(gid);

        // Creating the string to be used for the stat function
        sprintf(home_dir, "/home/%s", p);

        // Calling the stat() function on the home network for the user
        stat(home_dir, &s);

        printf("%d, %d, %s, %s, %ld\n", uid, gid, p, g->gr_name,(long) s.st_ino);
}

/*
    launch_func launchs the program and wait for it to terminate.
	take in a list of argumentsand forks the process 
	child process will run the user command
*/
int launch_func(char **args)
{
        pid_t pid, wpid;
        int status;

        pid = fork();
        // This is the child process
        if (pid == 0)
        {
				//execvp takes a program name and a vector of string arguments
				//and takes a program name instead of full file path
                if (execvp(args[0], args) == -1)
                {
                        perror("customsh");
                }

                exit(EXIT_FAILURE);
        }

        else if (pid < 0)
        {
                perror("customsh");
        }

        // This is the parent process
        else
        {
                do
                {
                  wpid = waitpid(pid, &status, WUNTRACED);
                }
                while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        return 1;
}

/*
    execute_func executes the shell built-in or launch program.       
*/

int execute_func(char **args)
{
        int i;

        // If no command has been entered
        if (args[0] == NULL)
        {
                return 1;
        }

        // Looping through the built-in command functions array
        for (i = 0; i < num_builtins(); i++)
        {
                //If the argument passed in matches one of the commands, run that command
                if (strcmp(args[0], builtin_str[i]) == 0)
                {
                        return (*builtin_func[i])(args);
                }
        }

        return launch_func(args);
}

/*
	read_line_func reads a line of input from user.
	Then return the line.
	Start with a block of memory and allocate more if needed.
 */
 
char *read_line_func(void)
{
        int bufsize = cust_RL_BUFSIZE;
        int position = 0;
        char *buffer = malloc(sizeof(char) * bufsize);
        int c;

		//check to see if allocation is exceeded and reallocate more space 
        if (!buffer)
        {
                fprintf(stderr, "customsh: allocation error\n");
                exit(EXIT_FAILURE);
        }

        while (1)
        {
                c = getchar();

                if (c == EOF || c == '\n')
                {
                        buffer[position] = '\0';
                        return buffer;
                }

                else
                {
                        buffer[position] = c;
                }
                position++;

                if (position >= bufsize)
                {
                        bufsize += cust_RL_BUFSIZE;
                        buffer = realloc(buffer, bufsize);
                        if (!buffer)
                        {
                                fprintf(stderr, "customsh: allocation error\n");
                                exit(EXIT_FAILURE);
                        }
                }
        }
}

/*
    split_line splits a line into individual elements
	using the same method as before for reallocating memory
	does not allow quoting or backslash escaping in the command line arguments
	uses white space to seperate arguments from each other
*/

char **split_line_func(char *line)
{
        int bufsize = cust_TOK_BUFSIZE, position = 0;
        char **tokens = malloc(bufsize * sizeof(char*));
        char *token;

        if (!tokens)
        {
                fprintf(stderr, "customsh: allocation error\n");
                exit(EXIT_FAILURE);
        }

        token = strtok(line, cust_TOK_DELIM);
        while (token != NULL)
        {
                tokens[position] = token;
                position++;

                if (position >= bufsize)
                {
                        bufsize += cust_TOK_BUFSIZE;
                        tokens = realloc(tokens, bufsize * sizeof(char*));
                        if (!tokens)
                        {
                                fprintf(stderr, "customsh: allocation error\n");
                                exit(EXIT_FAILURE);
                        }
                }

                token = strtok(NULL, cust_TOK_DELIM);
        }
        tokens[position] = NULL;
        return tokens;
}

/*
        main_loop getting input and executing it
*/
void main_loop(void)
{

        FILE *fp;
        char *line;
        char **args;
        int status;
        char whoami[100];
        const char *const red = "\e[38;5;196m";
        const char *const normal = "\033[0m";

        // Running the username command
        fp = popen("/usr/bin/whoami", "r");
        while(fgets(whoami, sizeof(whoami)-1, fp) != NULL)
        {
                // Removing the trailing new line character
                strtok(whoami, "\n");
        }

        pclose(fp);
        do
        {
                printf("%s%s@customsh >%s ",red ,whoami, normal);
                line = read_line_func();
                args = split_line_func(line);
                status = execute_func(args);

                free(line);
                free(args);
        }
        while (status);
}

/*
    Main entry point
*/


int main(int argc, char **argv)
{
		//Run main loop
        main_loop();

        //Perform shutdown/cleanup
        return EXIT_SUCCESS;
}


