#include "swag.h"
#include "inputProc.h"


//Takes a string array

//Prints out the contents of the array, used strictly for testing purposes

//Void
void printBuffy(char ** buff){
  int i = 0;
  while(buff[i++]){
    printf("->%s<-\n", buff[i - 1]);
  }
}

//Takes a string array

//Parses the arrray on semicolons
//The contents of the array between the semicolons are executed as individual lines

//Void
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

//Takes a string array guaranteed to have no semicolons

//First it checks for special commands like exit or redirects
//If there are none, it executes the line as a chain of pipes 
//(Regular commands are just a chain of pipes of length 1)

//Void
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

//Takes a string array guaranteed to have no semicolons or special commands

//Recursively chains pipes

//Void
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

//Takes a flag corresponding to how it should open the file,
//An int corresponding to whether it should read or write (rw),
//An index for a string array
//A string array

//The string array is guaranteed to have to have no semicolons, and a redirect command
//Handles the redirect by using pipes, based on what the flag and the rw are 

//Returns 0
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

//Takes a string array guaranteed to have no semicolons

//Checks to see if there are any redirect commands in the string array
//If so, it calls redirect with the appropriate arguments

//Returns 0 if it finds something, 1 otherwise
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


//Takes a string array guaranteed to have no semicolons

//Checks for exit and cd commands, then calls check_redirect

//Returns 0 if any special commands are found, 1 otherwise
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



//THE FOLLOWING FUNCTIONS ARE FOR READING USER INPUT


//Takes a string, and a string array

//Parses the string on spaces, feeding the non-null portions into the array

//Void
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

//Takes a string array and two string buffers

//Calls for user input and feeds it into parse_args

//Void
void setup_n_receive(char * * buff, char * cwd, char * inlin){
    getcwd(cwd, 256);
    printf("\n%s>> ", cwd);
    fgets(inlin, 256, stdin);
    inlin[strlen(inlin) - 1] = 0;
    parse_args(inlin, buff);
}
