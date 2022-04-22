void exec(char *args[], int *argc, int waitFor, char* currdir) {
	pid_t pid;
	int fd;
	char *fileName;
	int file = 0;
	switch (pid = fork()) { 
		case -1: 
			printf("Unable to fork\n");
			break;
		case 0: 
			if (*argc>=4) {
				if (strcmp(args[*argc-3], ">")==0) {
					file=1;
					fiilenfo(args, argc, &fileName);
					fd=open(fileName, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);
					if (fd < 0) {
						printf("Could not open the file\n");
						return;
					}
					dup2(fd, 1);
				}
				else if (strcmp(args[*argc-3], "<")==0) { 
					file=1;
					fiilenfo(args, argc, &fileName);
					fd = open(fileName, O_RDONLY);
					if (fd<0) {
						printf("Could not open the file\n");
						return;
					}
					dup2(fd, 0);
				}
			}
			childProcess(args, argc, NULL, currdir);
			printf("%s: command not found\n",args[0]);
			break;
		default: 
			parent_process(waitFor,pid);
			break;
	}
	if (file == 1) {
		close(fd);
	}
}