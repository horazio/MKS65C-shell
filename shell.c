#include "swag.h"

/*
static void sighandler(int signo){
  if(signo == SIGUSR1){
    
    printf("yeah smoetimes");
  }
}
*/

char **parse_args( char *line ){
  char **args = calloc(8, sizeof(char *));
  int i = 0;
  while( line ) {
    args[i] = strsep( &line, " " );
    i++;
  }
  return args;
}



int main(){
  //struct stat * buff = malloc(sizeof(struct stat));
  // signal(SIGUSR1, sighandler);
  
    while("st"){
      char cwd[256];
      getcwd(cwd, 256);
      printf("\n%s>> ", cwd);
      char inlin[256];
      fgets(inlin, 256, stdin);
      inlin[strlen(inlin) - 1] = 0;
      char **buff = parse_args(inlin);
      
      
      
      if(!strcmp(buff[0], "exit")){
	exit(0);
        }
      
      
      int i = 0;
      int fd;
	int squire = 0;
	int ff;
	int fds[2];

	
	while(buff[i]){
	  if(!strcmp(buff[i], "<")){
	    squire = 2;
            fd = open(buff[i + 1], O_RDONLY);
            if(fd == -1){
              printf("okok: %s\n", strerror(errno));
            }
            ff = dup(0);
            dup2(fd, 0);
            buff[i--] = NULL;
	  }
	  if(!strcmp(buff[i], ">>")){
	    squire = 1;
	    fd = open(buff[i + 1], O_WRONLY | O_APPEND);
	    if(fd == -1){
              printf("okok: %s\n", strerror(errno));
	    }
            ff = dup(1);
            dup2(fd, 1);
            buff[i--] = NULL;
	  }
	  if(!strcmp(buff[i], ">")){
	    squire = 1;
	    fd = open(buff[i + 1], O_WRONLY);
	    if(fd == -1){
	      printf("okok: %s\n", strerror(errno));
	    }
	    ff = dup(1);
	    dup2(fd, 1);
	    buff[i--] = NULL;
	  }
	  if(!strcmp(buff[i], "|")){
	    squire = 3;
	    if(pipe(fds) == -1){
	      printf("ok pipe error : %s\n", strerror(errno));
	    }
	    buff[i--] = NULL;
	  }
	  	  
	  i++;
	}
	
	
	
        int status = 0;
        int f = fork();

	
	wait(&status);
	
	char * path;



	
	
	if(!strcmp(buff[0], "cd")){
	  if(!f){
	    exit(0);
	  }else{
	    path = buff[1];
	    chdir(path);
	  }
	}
	
	
        if(!f){
	  if(squire == 3){
	    f = fork();
	   
	    if(f){
	      
	      close(fds[READ]);
	      dup2(fds[WRITE], 1);
	      execvp(buff[0], buff);
	    }
	    else{
	    
	      close(fds[WRITE]);
          
	      dup2(fds[READ], 0);
	 
	      buff = & buff[i + 1];
	      execvp(buff[0], buff);
	    }
	  }
	  execvp(buff[0], buff);
	}else{
	  if(squire == 1){
	    close(fd);
	    dup2(ff, 1);
	  }else if(squire == 2){
	    close(fd);
	    dup2(ff, 0);
	  }else if(squire == 3){
	    close(fds[READ]);
	    close(fds[WRITE]);
	  }
	}
        free(buff);
    }
    return 0;
    
}
