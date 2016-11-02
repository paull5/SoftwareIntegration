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
    Declare functions for commands
*/

int pwd_func(char **args);
int date_func(char **args);
int cd_func(char **args);
int help_func(char **args);
int logout_func(char **args);
int ifc_func(char **args);
int ud_func(char **args);
int ls_func(char **args);
int df_func(char **args);
int sh_func(char **args);

/*
     Global variables used for memory allocation in  read_line and split_line
*/


#define cust_TOK_BUFSIZE 64
#define cust_TOK_DELIM " \t\r\n\a"
#define cust_RL_BUFSIZE 1024

/* 
     Array to hold  a list of commands.
*/

char *builtin_str[] = {
        "cd",
        "help",
        "logout",
        "pw",
        "ifc",
        "dt",
        "ud",
        "ls",
	"df",
	"shell",
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
        &ls_func,
	&df_func,
	&sh_func,
};

int num_builtins()
{
        return sizeof(builtin_str) / sizeof(char *);
}

/*
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
    If ifc followed by an interface is entered that interface will bre displayed
*/

int ifc_func(char **args) 
{

	char command[100];

	//if no argument is passed
	//print eth0 as default
	if (args[1] == NULL) 
	{
		printf("\n");
		system("ifconfig eth0");

	//else if an argument is passed
	//print desired ifconfig interface
	}
	else 
	{
	    if (args[1] != NULL) {
	       	printf("\n");
			sprintf(command, "ifconfig %s",args[1]);
			system(command);

	}
	//else print input error
	else
	{
		fprintf(stderr, "customsh: invalid argument to \"ifc\"\n");
	}

    }

    return 1;
}

/*
    Print help function displays a lisy of allowed commands.
*/

int help_func(char **args)
{
	const char *const yellow = "\e[33m";


	if (args[1] == NULL) 
	{

		printf("%s", yellow);
        	printf("Type program names followed by any arguments, and press enter.\n");
        	printf("These are the built in commands:\n");
		printf("\n");
		printf("******************************************************************\n");
		printf("cd: Changes the current directory.\n"); 
		printf("pw: Displaysthe current working directory.\n");
		printf("ifc: Displays the ifconfig of the eth0 by default, you can specify your own interface.\n");
		printf("dt: Displays the current date.\n");
		printf("ud: Displays the userid, groupid, username and groupname.\n");
		printf("ls: Dispalys the content of the etc and bin directories.\n");
		printf("df: Displays the the free disk space.\n");
		printf("shell: Displays the shell where the user is currently and it's PID.\n");
		printf("logout: Exits the current shell and the system.\n");
		printf("******************************************************************\n");


	}
        return 1;
}

/*
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
    ls_func displaya a list of the content of the etc and bin directories
*/

int ls_func(char **args)
{

        FILE *fp;
        char path[1035];

        //Open the command for reading.
        fp = popen("/bin/ls /etc/", "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
        exit(1);
        }

        //Read the output a line at a time - output it.
        while (fgets(path, sizeof(path)-1, fp) != NULL) {
            printf("%s", path);
        }

        //close
        pclose(fp);

        return 1;
}

/*
    Displays free disk space
*/

int df_func(char **args)
{
        // If no argument is passed
        if(args[1] == NULL)
        {
                system("df");
        }

        else
        {
                system("df");
        }
        return 1;
}

/*
    sh_func displays shell and it's process id (PID)
*/

int sh_func(char **args)
{
        // If no argument is passed
        if(args[1] == NULL)
        {
                system("echo $SHELL\n");
		system("ps -p $$");

        }

        else
        {
                system("echo $SHELL\n");
		system("ps -p $$");

        }
        return 1;
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

        return 1;
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
	const char *const red = "\e[38;5;82m";
        const char *const blue = "\033[1;34m";
        const char *const normal = "\033[0m";

        // Running the username command
        fp = popen("/usr/bin/whoami", "r");
        while(fgets(whoami, sizeof(whoami)-1, fp) != NULL)
        {
                // Removing the trailing new line character
                strtok(whoami, "\n");
        }

        pclose(fp);
	printf("%sPlease type help to see a list of available commands:\n",red);

        do
        {
                printf("%s%s@customsh $%s ",blue ,whoami, normal);
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
