void fiilenfo(char *args[], int *argc, char **fileName) {
	*fileName = malloc(sizeof(char)*strlen(args[*argc-2]));
	strcpy(*fileName, args[*argc-2]);
	free(args[*argc-1]);
	free(args[*argc-2]);
	free(args[*argc-3]);
	*argc-=2;
	args[*argc-1]=NULL;
}