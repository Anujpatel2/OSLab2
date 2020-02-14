#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"

void cd(char *directory){
	int success;
	success = chdir(directory);
	if(success!=0){
		printf("Error: %s not found\n", directory);
	}

	char *current_dir[256] = { 0 };
    getcwd(current_dir, 256);
    setenv("PWD", current_dir, 1);
}

void clr(void){
	printf("\033[H\033[J"); 
}

void dir(){
	
	DIR *dir = opendir(".");
	
    struct dirent *contents; 

    if(dir != NULL){ 
    	             
        while((contents = readdir(dir)) != NULL){
            printf("%s\n", contents->d_name);
        }
        closedir(dir);
    }
}

void cmd_environ(void){		
	
	extern char **environ;
        
    int i = 0;

    while(environ[i] != NULL){ 
        printf("%s\n", environ[i]);
        i++;
    }
}

void echo(char *comment){

	printf("%s\n", comment);
}

void help(char help_dir[]){  
	
	char line[256];

	FILE *fp;
	fp = fopen(help_dir, "r"); 

	if(fp){
		while (fgets(line, 256, fp) != NULL) {
			printf("%s", line);
		}
		printf("\n");
	}
	fclose(fp);
}

void pause(void){
	
	do {
    	printf("Please Enter if you want to Continue"); 
    } while (getchar() != '\n');
}
