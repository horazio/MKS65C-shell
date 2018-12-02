#include "swag.h"
#include "inputProc.h"

void printBuffy(char ** buff){
  int i = 0;
  while(buff[i++]){
    printf("->%s<-\n", buff[i - 1]);
  }
}

void scanLines(char * * buff){
  int i = 0;
  while(buff[i]){
    if(!strcmp(buff[i], ";")){
      buff[i] = NULL;
      execLine(buff);
      buff = &buff[i + 1];
      i = -1;
    }
    i++;
  }
  execLine(buff);
}

void execLine(char * * buff){
  int f, status;
  if (checker(buff)){
     f = fork();
     wait(&status);
          
     if(!f){
        chain_pipe(buff);  
     }
        
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
                
                return redirect(O_WRONLY | O_TRUNC, WRITE, i, buff);
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


//INITIAL

void parse_args( char * line , char * * args){
    int i = 0;
    char * temp;
    while( line ) {
        temp = strsep( &line, " " );
        if(strlen(temp) > 0){
          args[i] = temp; 
          i++;
        }
    } 
}

void setup_n_receive(char * * buff, char * cwd, char * inlin){
    getcwd(cwd, 256);
    printf("\n%s>> ", cwd);
    fgets(inlin, 256, stdin);
    inlin[strlen(inlin) - 1] = 0;
    parse_args(inlin, buff);
}
