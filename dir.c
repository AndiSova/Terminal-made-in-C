void dir(char* currdir, int fl) {	
	char *curr_dir = NULL; 
	DIR *dp = NULL; 
	struct dirent *dptr = NULL; 
	unsigned int i = 0; 
	int totalEntries;
	struct dirent ** dir_entry;	
	char **contents;
	struct winsize w; 
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 
	curr_dir = currdir; 
	if(NULL == curr_dir) { 
		printf("\n dir: Could not get the working directory\n"); 
	} 
	dp = opendir((const char*)curr_dir);    
	if(NULL == dp) { 
		printf("\n dir: Could not open the working directory\n"); 
	} 
	if ( (totalEntries = scandir(curr_dir, &dir_entry, 0, alphasort)) < 0 )
		perror("dir: Could not scan the current directory");

	contents =  malloc(200 * sizeof(char*));
	int y = 0;
	for(i = 0; i < totalEntries; i++) { 
		if(dir_entry[i]->d_name[0] != '.') { 
			contents[y] = malloc(200 * sizeof(char));
			contents[y] = dir_entry[i]->d_name;
			y++;
		}
		else if(fl) {
			contents[y] = dir_entry[i]->d_name;
			y++;
		}
	} 
	int el = y;
	char *temp;
	temp = malloc(200 * sizeof(char*));
	for(int i = 0; i < el; i++) {
      	for(int j = i+1; j < el; j++){
         	if(strcasecmp(contents[i],contents[j]) > 0 ){
            	strcpy(temp, contents[i]);
            	strcpy(contents[i], contents[j]);
            	strcpy(contents[j], temp);
         	}
		}
    }
	for(int i = 0; i < el; i++) {
		printf("%s  ",contents[i]);
	}
	if( contents != NULL ) {
    	free(contents);
	}
	printf("\n");
}
