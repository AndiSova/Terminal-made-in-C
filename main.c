#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>  
#include <sys/wait.h>    
#include <sys/types.h>     
#include <dirent.h>       
#include <time.h>          
#include <pwd.h>          
#include <grp.h>           
#include <ftw.h>	   
#include <unistd.h> 	   
#include <fcntl.h>  
#include <sys/ioctl.h> 
#include <errno.h>
#include <ctype.h>
#include <err.h>
#include <limits.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

#include "exec.c" 
#include "fileinfo.c"
#include "process.c"
#include "parse.c"
#include "dir.c"
#include "cd.c"
#include "rm.c"
#include "uniq.c"

#define MAX_LINE 100

void clear_args(char *args[]) {
	int i=0;
	while (args[i]!=NULL) {
		free(args[i]);
		i++;
	}
}

int main() {
	int running = 1;
	char *args[MAX_LINE/2+1];
	int argc;
	char historyCmd[MAX_LINE]="";
	char command[MAX_LINE];
	int waitP;
	pid_t pid; 
	char * cmd = (char *)malloc(100 * sizeof(char));
	cmd = "HOME";
	chdir(getenv(cmd));
	char * path = (char *)malloc(100 * sizeof(char));
	getcwd(path, 100); 
	currdir = (char *)malloc(100*sizeof(char));
	strcpy(currdir, path);
	prevDir = (char *)malloc(100*sizeof(char));
	strcpy(prevDir, currdir);
	chdir(currdir);

	while (running==1) {
		printf("%s %s", currdir, ">");
		*command = "";
		scanf("%[^\n]s", command);
		waitP = 1;

		if (strcmp(command, "@")==0) {
			if (strcmp(historyCmd, "")==0)
			printf("No commands in history.\n");
			else {
				strcpy(command, historyCmd);
			}
		}

		char *subCmd=strchr(command, '|');
		int pipeNum = 0;
		if(strchr(command, '|') != NULL) {
			pipeNum += 1;
			for (int i = 0; i < strlen(command); i++){
        		if (command[i] == '|') {
            		pipeNum+=1;
        		}
    		}
		}

		char* chCmd;
		char *Cmd[pipeNum];
		if(pipeNum > 1) {
			pipeNum -= 1;
			chCmd = strtok (command,"|");
			int k = 0;
			while (chCmd != NULL){
				Cmd[k] = chCmd;	  
				chCmd = strtok (NULL, "|");
				k++;
			}
		}

		if (strcmp(command, "exit")==0) {
			running=0;
			kill(0,SIGKILL);
		}
		else {
			if (subCmd!=NULL) {
				if(pipeNum == 1) {
					char *cmd;
					if (strcmp(command, "")!=0) {
						strcpy(historyCmd, command);
					}
					cmd=strtok(command, "|");
					subCmd += 2;
					parsing(cmd, args, &argc, &waitP);
					switch (pid=fork()) {
						case -1:
							printf("Unable to fork\n");
							break;
						case 0:
							childProcess(args, &argc, subCmd, currdir);
							break;
						default:
							parent_process(waitP,pid);
							break;
					}
				}
				else {
					// Does't work for multiple pipes
					for(int i = 0; i <= pipeNum ; ++i) {
						char *cmd;
						if (strcmp(command, "")!=0) {
							strcpy(historyCmd, command);
						}
						cmd=strtok(command, "|");
						int j = i + 1;
						parsing(cmd, args, &argc, &waitP);
						switch (pid=fork()) {
							case -1:
								printf("Unable to fork\n");
								break;
							case 0:
								childProcess(args, &argc, Cmd[j], currdir);
								break;
							default:
								parent_process(waitP,pid);
								break;
						}
					}
				}
			}
			else {
				if (strcmp(command, "")!=0) {
					if (strcmp(command, "@")!=0) {
						strcpy(historyCmd, command);
					}
				}	
				parsing(command, args, &argc, &waitP);
				if (strcmp(args[0],"rm") == 0) {
					rm(args, argc);
				}
				else if (strcmp(args[0], "cd") == 0) {
					cd(args, argc);
				}
				else if (strcmp(args[0], "dir") == 0) {
					dir(currdir, 0);
				}
				else if (strcmp(args[0], "uniq") == 0) {
					uniq(argc, args);
				}
				else {
					exec(args, &argc, waitP, currdir);
				}
			}
		}
		strcpy(command, "");
		clear_args(args);
		getchar();
	}
	return 0;
}