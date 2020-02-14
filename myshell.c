/*
 * Operating Systems Lab 2 - Shell commands
 *
 * 			Harsh Ghoswami (100654736) 	
 * 			Preet Khasakia (100653168)
 *			Anuj Patel (100651964)
 *			Vraj Shah (100658669)
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "myshell.h"
#include "utility.h"

#define BUFFER_LENGHT 256
#define HELP_LENGTH 1024

char help_direc[HELP_LENGTH];

void locateReadMe(){
	getcwd(help_direc, sizeof(help_direc));
	strcat(help_direc, "/readme"); 
}

void printCurDirec(){
	char cur_direc[HELP_LENGTH];
	getcwd(cur_direc, sizeof(cur_direc));
	printf("%s > ", cur_direc); 
}

void tokenize(char *buffer, char *command, char *arg){
	char *newLine = strstr(buffer, "\n");
	if(newLine != NULL){
		*newLine = 0;
	}
	if(newLine==buffer){
		return;
	}

	char *tokens = strtok(buffer, " ");
	strncpy(command, tokens, BUFFER_LENGHT);
	tokens = strtok(NULL, " ");

	while(tokens != NULL){
		strcat(arg, tokens);
		tokens = strtok(NULL, " ");
		if(tokens!=NULL){
			strcat(arg, " ");
		}
	}
}

void execCommands(char *command, char *arg){

	if (strcmp(command, "cd") == 0){
		cd(arg); 
	} 
	else if(strcmp(command, "clr") == 0){   
		clr();                                                               
	} else if(strcmp(command, "dir") == 0){       
		dir();
	} else if(strcmp(command, "environ") == 0){
		cmd_environ();
	} else if(strcmp(command, "echo") == 0){
		echo(arg);
    	} else if(strcmp(command, "help") == 0){
		help(help_direc);
	} else if(strcmp(command, "pause") == 0){              
		pause();
	} else{
		
        if(strstr("./",command)==NULL){
		for(int i=BUFFER_LENGHT-3; i>=0; i--){
			command[i+2] = command[i];
		}
		command[0] = '.';
		command[1] = '/';
	}
	if(access(command, F_OK)==0){
		pid_t pid = fork();;    
		printf("PID: %d \n",pid);
		if(pid==0){ 
                if(strcmp(arg, "&") == 0){
			printf(" BACKGROUND\n");
			freopen( "/dev/null", "r", stdin);
			freopen( "/dev/null", "w", stdout);
			freopen( "/dev/null", "w", stderr);
                }  
		char *parent[BUFFER_LENGHT] = { 0 };
                *parent = getenv("Shell");
                setenv("Parent", parent, 1);
		int status = system(command);
		if(status==-1){
			printf("Error running %s\n", command);
		}

		exit(0);
	} else {
		if (pid==-1){
			perror("fork\n");
		} 
	}
} else {
        	printf("%s, not found\n", command);
        }
    }
}

void batchfile(char* arg){

	char tempCommand[BUFFER_LENGHT] = { 0 };
	char tempArg[BUFFER_LENGHT] = { 0 };
	
	
   	FILE *fp;
   	char line[BUFFER_LENGHT];
	
    
   	
	fp = fopen(arg, "r");

	
    	if(fp == NULL){
    		printf("Error reading file.\n");
    	} else{
    	while(fgets(line, BUFFER_LENGHT, fp)){
     		tokenize(line, tempCommand, tempArg);		
     		execCommands(tempCommand, tempArg);		
    	}
        fclose(fp); 
    }
}

int main(int argc, char *argv[])
{
    char *origin[BUFFER_LENGHT] = { 0 };
    readlink("/proc/self/exe", origin, BUFFER_LENGHT);
    setenv("Shell", origin, 1);

    char *current_dir[BUFFER_LENGHT] = { 0 };
    getcwd(current_dir, BUFFER_LENGHT);
    setenv("PWD", current_dir, 1);

    char buffer[BUFFER_LENGHT] = { 0 }; 

    locateReadMe(); 

    
    if(argc == 2){
        batchfile(argv[1]);
    } else{
    	printCurDirec(); 
      
        while (fgets(buffer, BUFFER_LENGHT, stdin) != NULL)
        {
           
            char command[BUFFER_LENGHT] = { 0 };
            char arg[BUFFER_LENGHT] = { 0 };

                             
        	tokenize(buffer, command, arg);

        	if (strcmp(command, "quit") == 0){
                return EXIT_SUCCESS;
            } else {
           
            	execCommands(command, arg);
        	}
        	
        	printCurDirec();
        }
    }
    return EXIT_SUCCESS;
}
