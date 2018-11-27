#include "swag.h"

static void sighandler(int signo){
  if(signo == SIGUSR1){
    
    printf("yeah smoetimes");
  }
}


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
    signal(SIGUSR1, sighandler);
    
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
	
	while(buff[i]){
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
	  execvp(buff[0], buff);
	}else{
	  if(squire){
	    close(fd);
	    dup2(ff, 1);
	  }
	}
        free(buff);
    }
    return 0;
    
}
