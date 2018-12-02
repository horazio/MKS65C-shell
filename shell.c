#include "swag.h"

char ** parse_args( char *line ){
    char **args = calloc(8, sizeof(char *));
    int i = 0;
    while( line ) {
        args[i] = strsep( &line, " " );
        i++;
    }
    return args;
}


int is_last(int i, char ** buff){
  while(buff[i]){
     if(!strcmp(buff[i], "|")){
       return 0;
     }
  }
  return 1;
}


void printBuffy(char ** buff){
  int i = 0;
  while(buff[i++]){
    printf("'%s'\n", buff[i - 1]);
  }
}



void chain_pipe(char ** buff){
  int i = 0;
  int f;
  int fds[2];
  int status;
  int cpy;
  
  
  while(buff[i]){
    if(!strcmp(buff[i], "|")){
      pipe(fds);
      //printf("asdf\n");
      buff[i] = NULL;
      f = fork();
      wait(&status);
      if(!f){
        //printBuffy(buff);
        close(fds[READ]);
        dup2(fds[WRITE], 1);
        //buff = &buff[i + 1];
        execvp(buff[0], buff);
      }else{
        close(fds[WRITE]);
        dup2(fds[READ], 0);
        buff = &buff[i + 1];
        //printBuffy(buff);
        //printf("asdf\n");
        chain_pipe(buff);
      }
      //close(fds[READ]);
      //close(fds[WRITE]);
    }
    i++;
  }
    //printf("poire\n");
    //printBuffy(buff);
    execvp(buff[0], buff);
}

/*
void chain_pipe(char ** buff){
  int i = 0;
  int f;
  int status = 0;
  int fds[2];

  while(buff[i]){

    if(!strcmp(buff[i], "|")){
        buff[i] = NULL;

          pipe(fds);
          f = fork();
          wait(&status);
          if(!f){

              printBuffy(buff);

              close(fds[READ]);
              dup2(fds[WRITE], 1);

              execvp(buff[0], buff);
          }else{
              dup2(fds[READ], 0);

              close(fds[READ]);
              close(fds[WRITE]);

              buff = &buff[i + 1];

            if( is_last(i, &buff[i + 1]) ){
              buff = & buff[i + 1];
              printBuffy(buff);
              execvp(buff[0], buff);
             }

              i = -1;
          }
        }


    i++;
  }
}
*/

char * * setup_n_receive(char * cwd, char * inlin){
    getcwd(cwd, 256);
    printf("\n%s>> ", cwd);
    fgets(inlin, 256, stdin);
    inlin[strlen(inlin) - 1] = 0;
    return parse_args(inlin);
}


int main(){

    char cwd[256];
    char inlin[256];
    char * path;
    int i = 0;
    int fd;
    int squire = 0;
    int ff;
    int status = 0;
    int new;
    int f;
    int fds[2];
    char * * buff;

    while(1){
        buff = setup_n_receive(cwd, inlin);

        if(!strcmp(buff[0], "exit")){
            exit(0);
        }


        if(!strcmp(buff[0], "cd")){
                path = buff[1];
                chdir(path);
                buff[0] = NULL;
        }

        i = 0;
        squire = 0;

  /*
        f = fork();
        wait(&status);
        if(!f){
          chain_pipe(buff);
        }
        printf("asdfasdf\n");
        */


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
            }

            i++;
        }


        //forktime

        f = fork();
        wait(&status);



        if(!f){
            if(squire == 3){
              chain_pipe(buff);  
            }else{
              execvp(buff[0], buff);
            }
        }else{
            if(squire == 1){
                close(fd);
                dup2(ff, 1);
            }else if(squire == 2){
                close(fd);
                dup2(ff, 0);
            }
        }
        free(buff);
    }
    return 0;

}
