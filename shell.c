#include "swag.h"

void printBuffy(char ** buff){
  int i = 0;
  while(buff[i++]){
    printf("->%s<-\n", buff[i - 1]);
  }
}


void parse_args( char * line , char * * args){
    int i = 0;
    while( line ) {
        args[i] = strsep( &line, " " );
        i++;
    } 
}





void chain_pipe(char ** buff){
  
  int i = 0;
  int f;
  int fds[2];
  int status;
  
  while(buff[i]){
    if(!strcmp(buff[i], "|")){
      pipe(fds);
      buff[i] = NULL;
      f = fork();
      wait(&status);
      if(!f){
        close(fds[READ]);
        dup2(fds[WRITE], 1);
        execvp(buff[0], buff);
      }else{
        close(fds[WRITE]);
        dup2(fds[READ], 0);
        buff = &buff[i + 1];
        chain_pipe(buff);
      }
    }
    i++;
  }
    
    execvp(buff[0], buff);
}



void setup_n_receive(char * * buff, char * cwd, char * inlin){
    getcwd(cwd, 256);
    printf("\n%s>> ", cwd);
    fgets(inlin, 256, stdin);
    inlin[strlen(inlin) - 1] = 0;
    parse_args(inlin, buff);
}


int redirect(int flag, int rw, int i, char * * buff){
  int ff, fd, status, f;
  fd = open(buff[i + 1], flag);
  if(fd == -1){
      printf("okok: %s\n", strerror(errno));
  }  
  ff = dup(rw);
  dup2(fd, rw);
  buff[i] = NULL;
  f = fork();
  wait(&status);
  if(!f){
      execvp(buff[0], buff); 
  }      
  close(fd);
  dup2(ff, rw);
  return 0;
}


int check_redirect(char * * buff){
  int i = 0;
  while(buff[i]){
            if(!strcmp(buff[i], "<")){
                return redirect(O_RDONLY, READ, i, buff);
            }
            if(!strcmp(buff[i], ">>")){
                return redirect(O_WRONLY | O_APPEND, WRITE, i, buff);
                
            }
            if(!strcmp(buff[i], ">")){
                return redirect(O_WRONLY, WRITE, i, buff);
            }
            i++;
        }
  return 1;
}


int checker(char * * buff){
  if(!strcmp(buff[0], "exit")){
    exit(0);
    return 0;
  }

  if(!strcmp(buff[0], "cd")){
    chdir(buff[1]);
    return 0;
  }
    
  return check_redirect(buff);
}



int main(){
    int f, status;
    char * * buff;
    char cwd[256];
    char inlin[256];

    while(1){
      free(buff);
      char * * buff = calloc(8, sizeof(char *));
      setup_n_receive(buff, cwd, inlin);
        
      if (checker(buff)){
        
          f = fork();
          wait(&status);
          
          if(!f){
           chain_pipe(buff);  
          }
        
        }
    }
  
    //not sure why this is even here but ok
    return 0;

}
