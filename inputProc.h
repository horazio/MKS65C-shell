void printBuffy(char ** buff);

void parse_args( char * line , char * * args);

void setup_n_receive(char * * buff, char * cwd, char * inlin);

void chain_pipe(char ** buff);

int redirect(int flag, int rw, int i, char * * buff);
  
int check_redirect(char * * buff);

int checker(char * * buff);

void execLine(char * * buff);

void scanLines(char * * buff);