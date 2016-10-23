Paul Lawlor
c13764501@mydit.ie
Software Ingtegration assignment
Create a custom shell

Steps to create a custom shell in linux
1. Create a user with adduser "username" (replace username with the name of your user).
2. Give them a password with passwd "username" (replace username with the name of your user).
3. Change directory with cd to /usr/bin.
4. Clone the following repository form git hub into /usr/bin   
    https://github.com/paull5/SoftwareIntegration.git
5. Change directory into SoftwareIntegration.
6. Complie the custom_sh.c file with sudo gcc -o custom_sh custom_sh.c
7. Change the shell of the user to that of the custom shell with
    sudo chsh -s /usr/bin/SoftwareIntegration custom_sh username (replace username with the name of your user)
8. The user will now have their own restricted shell

Test the program
1. Logout of the current shell with exit command.
2. Login with the credentials of the user you have just created .
3. You should now be logged in and have a custom shell.

