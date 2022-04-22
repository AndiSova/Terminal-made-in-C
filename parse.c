void parsing(char command[], char *args[], int *argc, int *waitFor) {
	char *arg;
	int i=0;
	char svd[MAX_LINE];

	strcpy(svd, command);

	arg=strtok(command, " ");
	while (arg!=NULL) {
		args[i] = malloc(sizeof(char)*strlen(arg));
		strcpy(args[i], arg);
		arg=strtok(NULL, " ");
		i++;
	}

	if (strcmp(args[i-1], "&")==0) {
		free(args[i-1]);
		i--;
		*waitFor=0;
	}

	args[i]=NULL;
	i++;

	strcpy(command, svd);
	*argc=i;
}