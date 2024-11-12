#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	for (int i = 0; i < N; i++)
	{
		if (strcmp(allowed[i], cmd) == 0)
		{
			return 1;
		}
	}
	// return 0 otherwise
	
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {
	
	// Print prompt
	fprintf(stderr,"rsh>");

	// Get input and check for error
	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// Initialize Empty Token array
	char* token_array[25] = {0};

	// Find initial token
    char* token = strtok(line, " ");
    int current_token_index = 0;

	// Find Tokens until strtok() returns NULL
	while (token != NULL)
	{
		// Allocate memory for the token in token_array
		token_array[current_token_index] = (char *)malloc(strlen(token) + 1);

		// Check if Malloc executed properly
		if (token_array[current_token_index] == NULL)
		{
			// Cry
			perror("malloc failed");
            exit(EXIT_FAILURE);
		}

		// Copy over the string contents
		strcpy(token_array[current_token_index], token);
		current_token_index ++;
		token = strtok(NULL, " ");
	}

	// Null-terminate the token array
	token_array[current_token_index] = NULL;

	// Verify that first token is a command
	char* command = token_array[0];
	if (isAllowed(command) == 0)
	{
		printf("NOT ALLOWED!\n");
		continue;
	}

	// Add code to spawn processes for the first 9 commands

	// Initialize PID, spawn attributes and status variables.
	pid_t PID;
    int status;

    posix_spawnattr_t attr;
    posix_spawnattr_init(&attr);

	// And add code to execute cd, exit, help commands

	// Exit
	if (strcmp(command, "exit") == 0)
	{
		return 0;
	}
	// Help
	else if (strcmp(command, "help") == 0)
	{
		printf( "The allowed commands are:\n"
				"1: cp\n"
				"2: touch\n"  
				"3: mkdir\n"  
				"4: ls\n"
				"5: pwd\n"  
				"6: cat\n"  
				"7: grep\n"  
				"8: chmod\n"  
				"9: diff\n"  
				"10: cd\n"  
				"11: exit\n"  
				"12: help\n");
	}

	// Spawn process
	else
	{	
		if (posix_spawnp(&PID, command, NULL, &attr, token_array, environ) != 0) 
		{
        	perror("spawn failed");
        	exit(EXIT_FAILURE);
		}
			
		// Wait for the spawned process to terminate
    	if (waitpid(PID, &status, 0) == -1) 
		{
        	perror("waitpid failed");
        	exit(EXIT_FAILURE);
    	}

    	// Destroy spawn attributes
    	posix_spawnattr_destroy(&attr);
		
	}
    }
    return 0;
}
