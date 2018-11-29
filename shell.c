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

void chain_pipe(char ** buff){
  int i = 0;
  int f;
  int status = 0;
  int fds[2];
  
  while(buff[i]){
    if(!strcmp(buff[i], "|")){
        buff[i] == NULL;
        f = fork();
        pipe(fds);
        printf("ok\n");
        wait(&status);
        if(!f){
            close(fds[READ]);
            dup2(fds[WRITE], 1);
            execvp(buff[0], buff);
        }else{
            dup2(fds[READ], 0);
            buff = &buff[i];
            if( is_last(i, &buff[1]) ){
                close(fds[WRITE]);
                execvp(buff[1], buff);
            }
        }
    }
    i++;
  }
}



int main(){


    while(1){
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
  
        //chain_pipe(buff);
        //buff[0] = NULL;
      
      
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

        int new;

        if(!f){
            if(squire == 3){
                if(pipe(fds) == -1){
                    printf("ok pipe error : %s\n", strerror(errno));
                }
                new = fork();
                if(new){

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
            }
        }
        free(buff);
    }
    return 0;

}
