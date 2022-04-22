int removedir(char *path){   		
   DIR *d = opendir(path);  		
   size_t p_len = strlen(path);  
   int r = -1;  					
   if (d){    						
      struct dirent *p;		
      r = 0;   							
      while (!r && (p=readdir(d))){    
         int r2 = -1;  			
         char *buf; 					
         size_t len;					
         if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")){   
            continue;  
         }
         len = p_len + strlen(p->d_name) + 2;  
         buf = malloc(len);  
         if (buf){          
            struct stat statbuf;  
            snprintf(buf, len, "%s/%s", path, p->d_name); 
            if (!stat(buf, &statbuf)){    
               if (S_ISDIR(statbuf.st_mode)){    
                  r2 = removedir(buf);   
               }
               else{
                  r2 = unlink(buf);   
               }
            }
            free(buf);     
         }
         r = r2; 
      }
      closedir(d);  
   }
   if (!r){
      r = rmdir(path);    
   }
	return r;
}

void rm(char * argv[], int argc) { 
   if(argc <= 2) {
      printf("rm: missing operand\n");
   }
   else if(argc <= 3 && ((strcmp(argv[1],"-r") == 0 || strcmp(argv[1],"-R") == 0 || strcmp(argv[1],"-v") == 0 || strcmp(argv[1],"-i") == 0))) {
      printf("rm: missing operand\n");
   }
   else {
      int n = argc - 1;
      char* cm = argv[1];
      char line[] = "-";
      int x;
      x = strcspn (cm,line);
      if(strstr(argv[1],"--") != 0 && x == 0) {
         fprintf(stderr, "rm: unrecognized option '%s'\n", argv[1]);
      }
      else if(strstr(argv[1],"-") != 0 && (strcmp(argv[1],"-r") != 0 && strcmp(argv[1],"-R") != 0 && strcmp(argv[1],"-v") != 0 && strcmp(argv[1],"-i") != 0) && x == 0) {
         char* er = argv[1];
         er = strtok(er,"-");
         fprintf(stderr, "rm: invalid option -- '%s'\n", er);
      }
      else {
         if((strcmp(argv[1],"-r")==0 && strcmp(argv[0],"rm")==0) || (strcmp(argv[1],"-R")==0 && strcmp(argv[0],"rm")==0)){
            struct stat is_dir;
            stat(argv[2], &is_dir);
            if(S_ISREG(is_dir.st_mode)) {
               DIR *d  = opendir(argv[2]);
               int ret = remove(argv[2]);
               if( ret != 0 ) {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                  perror("");
               }
            }
            else {
               int d = removedir(argv[2]);
               if(d!=0) {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                  perror("");
               }
            }
         }
         else if(strcmp(argv[1],"-v")==0){	
            struct stat is_dir;
            stat(argv[2], &is_dir);
            if(S_ISREG(is_dir.st_mode)) {
               DIR *d = opendir(argv[2]);
               int ret=remove(argv[2]);
               if( ret != 0 ) {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                  perror("");
               }
               else {
                  printf( "removed '%s'\n", argv[2] );
               }
            }
            else {
               if( access( argv[2], F_OK ) == 0 ) {
                  fprintf(stderr, "rm: cannot remove %s: Is a directory\n", argv[2]);
               } else {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                  perror("");
               }
            }	 		
         } 
         else if(strcmp(argv[1],"-i")==0) {
            struct stat is_dir;
            stat(argv[2], &is_dir);
            if(S_ISREG(is_dir.st_mode)) {
               if( access( argv[2], F_OK ) != 0 ) {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                  perror("");
               }
               else {
                  printf("rm: remove regular file '%s'? ", argv[2]);
                  char response[MAX_LINE];
                  scanf("%s", response);
                  if(strcmp(response,"y") == 0 || strcmp(response,"yes") == 0 || strcmp(response,"Y") == 0) {
                     DIR *d = opendir(argv[2]);
                     int ret = remove(argv[2]);
                     if( ret != 0 ) {
                        fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                        perror("");
                     }
                  }
               }
            }
            else {
               if( access( argv[2], F_OK ) == 0 ) {
                  fprintf(stderr, "rm: cannot remove %s: Is a directory\n", argv[2]);
               } else {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                  perror("");
               }
            }		
         }
         else {
            struct stat is_dir;
            stat(argv[1], &is_dir);
            if(S_ISREG(is_dir.st_mode)) {
               DIR *d  = opendir(argv[1]);
               int ret = remove(argv[1]);
               if( ret != 0 ) {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[2]);
                  perror("");
               }
            }
            else {
               if( access( argv[1], F_OK ) == 0 ) {
                  fprintf(stderr, "rm: cannot remove %s: Is a directory\n", argv[1]);
               } else {
                  fprintf(stderr, "rm: cannot remove %s: ", argv[1]);
                  perror("");
               }
            }
         } 
         
         if(n > 2 && ((strcmp(argv[1],"-r")==0 || strcmp(argv[1],"-R") == 0) || (strcmp(argv[1],"-v") == 0) || (strcmp(argv[1],"-i") == 0))) {
            for(int i = 3; i < n; i++) {
               if((strcmp(argv[1],"-r")==0 && strcmp(argv[0],"rm")==0) || (strcmp(argv[1],"-R")==0 && strcmp(argv[0],"rm")==0)){
                  struct stat is_dir;
                  stat(argv[i], &is_dir);
                  if(S_ISREG(is_dir.st_mode)) {
                     DIR *d  = opendir(argv[i]);
                     int ret = remove(argv[i]);
                     if( ret != 0 ) {
                        fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                        perror("");
                     }
                  }
                  else {
                     int d=removedir(argv[i]);
                     if(d!=0) {
                        fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                        perror("");
                     }
                  }
               }

               else if(strcmp(argv[1],"-v")==0){	
                  struct stat is_dir;
                  stat(argv[i], &is_dir);
                  if(S_ISREG(is_dir.st_mode)) {
                     DIR *d = opendir(argv[i]);
                     int ret=remove(argv[i]);
                     if( ret != 0 ) {
                        fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                        perror("");
                     }
                     else {
                        printf( "removed '%s'\n", argv[i] );
                     }
                  }
                  else {
                     if( access( argv[i], F_OK ) == 0 ) {
                        fprintf(stderr, "rm: cannot remove %s: Is a directory\n", argv[i]);
                     } else {
                        fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                        perror("");
                     }
                  } 		
               } 
               else if(strcmp(argv[1],"-i")==0) {
                  struct stat is_dir;
                  stat(argv[i], &is_dir);
                  if(S_ISREG(is_dir.st_mode)) {
                     if( access( argv[i], F_OK ) != 0 ) {
                        fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                        perror("");
                     }
                     else {
                        printf("rm: remove regular file '%s'? ", argv[i]);
                        char response[MAX_LINE];
                        scanf("%s", response);
                        if(strcmp(response,"y") == 0 || strcmp(response,"yes") == 0 || strcmp(response,"Y") == 0) {
                           DIR *d = opendir(argv[i]);
                           int ret = remove(argv[i]);
                           if( ret != 0 ) {
                              fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                              perror("");
                           }
                        }
                     }	    
                  }

                  else {
                     if( access( argv[i], F_OK ) == 0 ) {
                        fprintf(stderr, "rm: cannot remove %s: Is a directory\n", argv[i]);
                     } else {
                        fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                        perror("");
                     }
                  }
               } 
            }
         }
         else {
            for(int i = 2; i < n; i ++) {
               struct stat is_dir;
               stat(argv[i], &is_dir);
               if(S_ISREG(is_dir.st_mode)) {
                  DIR *d  = opendir(argv[i]);
                  int ret = remove(argv[i]);
                  if( ret != 0 ) {
                     fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                     perror("");
                  }
               }
               else {
                  if( access( argv[i], F_OK ) == 0 ) {
                     fprintf(stderr, "rm: cannot remove %s: Is a directory\n", argv[i]);
                  } else {
                     fprintf(stderr, "rm: cannot remove %s: ", argv[i]);
                     perror("");
                  }
               }
            }
         }
      }
   }
}
