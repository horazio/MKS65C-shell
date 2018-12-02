#include "swag.h"
#include "inputProc.h"

int main(){
    char * * buff;
    char cwd[256];
    char inlin[256];

    while(1){
      free(buff);
      char * * buff = calloc(8, sizeof(char *));
      setup_n_receive(buff, cwd, inlin);
      
      execLine(buff);
      
    }
  
    //not sure why this is even here but ok
    return 0;

}
