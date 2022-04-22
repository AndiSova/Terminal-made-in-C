#define MAX_LINE 100

void parent_process(int waitFor, pid_t id) {
	int stat;
	if (waitFor == 1) {
		waitpid(id, &stat, 0);
	}
}

void childProcess(char *args[], int *argc, char *pipeCmd, char* currdir) {
	pid_t pid; 
	char *nargs[MAX_LINE/2+1];
	int waitFor = 1;
	int nargc;
	int pipefd[2];
	if (pipeCmd!=NULL) {
		if (pipe(pipefd) == -1) {
			printf("Unable to pipe\n");
			return;
		}
		switch (pid=fork()) {
			case -1:
				printf("Unable to fork\n");
				break;
			case 0:
				close(pipefd[0]);
				dup2(pipefd[1], 1);
				childProcess(args, argc, NULL, currdir);
				close(pipefd[1]);
				break;
			default:
				parent_process(waitFor,pid);
				close(pipefd[1]);
				dup2(pipefd[0], 0);
				parsing(pipeCmd, nargs, &nargc, &waitFor);
				childProcess(nargs, &nargc, NULL, currdir);
				close(pipefd[0]);
				break;
		}		
	}
	else {
		if(strcmp(args[0],"dir") == 0) {
			dir(currdir, 0);
			exit(0);
		}
		else if (strcmp(args[0],"rm") == 0) {
			rm(args, *argc);
			exit(0);
		}
		else if (strcmp(args[0], "uniq") == 0) {
			uniq(*argc, args);
			exit(0);
		}
		else {
			execvp(args[0],args);
		}
	}
}
