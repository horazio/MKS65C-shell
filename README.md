# MKS65C-shell

Features:
-> This shell is indifferent to extra spaces
-> Multiple commands can be seperated with semicolons
-> Redirection is supported ('>', '<', '>>', but not '<<')
-> Pipes can be chained
-> This shell has a maximum argument length of 20


I briefly attempted to implement tab completion, but I soon realized it was going to be more work than I could handle


A copy of every function header:


void printBuffy(char ** buff);
  -> Takes a string array
  -> Prints out the contents of the array, used strictly for testing purposes
  -> Void

void scanLines(char * * buff);
  -> Takes a string array
  -> Parses the arrray on semicolons
     The contents of the array between the semicolons are executed as individual lines
  -> Void

void execLine(char * * buff);
  -> Takes a string array guaranteed to have no semicolons
  -> First it checks for special commands like exit or redirects
     If there are none, it executes the line as a chain of pipes 
     (Regular commands are just a chain of pipes of length 1)
  -> Void

void chain_pipe(char ** buff);
  -> Takes a string array guaranteed to have no semicolons or special commands
  -> Recursively chains pipes
  -> Void

int redirect(int flag, int rw, int i, char * * buff);
  -> Takes a flag corresponding to how it should open the file,
     An int corresponding to whether it should read or write (rw),
     An index for a string array
     A string array
  -> The string array is guaranteed to have to have no semicolons, and a redirect command
     Handles the redirect by using pipes, based on what the flag and the rw are 
  -> Returns 0
  
int check_redirect(char * * buff);
  -> Takes a string array guaranteed to have no semicolons
  -> Checks to see if there are any redirect commands in the string array
     If so, it calls redirect with the appropriate arguments
  -> Returns 0 if it finds something, 1 otherwise

int checker(char * * buff);
  ->Takes a string array guaranteed to have no semicolons
  ->Checks for exit and cd commands, then calls check_redirect
  ->Returns 0 if any special commands are found, 1 otherwise

void parse_args( char * line , char * * args);
  -> Takes a string, and a string array
  -> Parses the string on spaces, feeding the non-null portions into the array
  -> Void

void setup_n_receive(char * * buff, char * cwd, char * inlin);
  -> Takes a string array and two string buffers
  -> Calls for user input and feeds it into parse_args
  -> Void
