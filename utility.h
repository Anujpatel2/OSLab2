#ifndef UTILITY_H_
#define UTILITY_H_
#include "utility.c"




extern void cd(char *directory);
                                  


extern void clr(void);
						 

extern void dir();  


extern void cmd_environ(void); 


extern void echo(char *comment); 


extern void help(char help_dir[]);

extern void pause(void);    


#endif /* UTILITY_H_ */
