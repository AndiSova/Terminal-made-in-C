char * currdir = NULL; 
char * prevDir = NULL;

char * strDir(char * dirr){
	char * prev = (char * )malloc(100*sizeof(char));
	strcpy(prev, dirr);	
	return prev;
}

char * chain(const char *a, const char *b) {
	char *newStr = malloc(strlen(a) + strlen(b) + 1);
	strcpy(newStr, a);
	strcat(newStr, b);
	return newStr;
}

char * cd(char * args[], int argc){
	if(argc <= 2) {
		printf("cd: missing operand\n");
	}
	else {
		char * buff = (char *)malloc(100*sizeof(char));	
		char * temp = (char *)malloc(100*sizeof(char));	
		strcpy(temp, currdir);
		temp = chain(temp, "/"); 
		temp = chain(temp, args[1]);
		if((chdir(temp) == -1) != 0){
			snprintf(buff, 100, "cd: %s: No such file or directory", args[1]);
			printf("cd: %s: Not a directory\n", args[1]); 
			return buff;
		}

		if(strcmp(args[1], ".") == 0) {
			return currdir;
		}

		if(args[1] == "") {
			chdir(getenv("HOME"));
			getcwd(currdir, 100);
			return currdir;
		}
		
		currdir = temp;	
		return currdir;
	}
}
