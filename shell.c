#include "swag.h"
#include "inputProc.h"

//Runs a forever while loop prompting the user for input
//Then executs the user's commands
int main(){
    char * * buff;
    char cwd[256];
    char inlin[256];

    while(1){
      free(buff);
      char * * buff = calloc(20, sizeof(char *));
      setup_n_receive(buff, cwd, inlin); 
      scanLines(buff);
    }
  
    //not sure why this is even here but ok
    return 0;

}
