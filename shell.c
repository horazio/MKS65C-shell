#include "swag.h"




char **parse_args( char *line ){
    char **args = calloc(8, sizeof(char *));
    int i = 0;
    while( line ) {
        args[i] = strsep( &line, " " );
        i++;
    }
    return args;
}

/*
int main()
{

    char in[] = "ls -a -l ..";
    printf("the line is: \"%s\"\nParsed:\n", in);
    char **jeff = parse_args(in);
    int i = 0;
    //printf("OOF\n");
    while( jeff[i] )
        printf("\"%s\"\n", jeff[i++]);
    free(jeff);
    return 0;
}
*/

int main(){
    //struct stat * buff = malloc(sizeof(struct stat));

    while("st"){
        char cwd[256];
        getcwd(cwd, 256);
        printf(">>\n%s", cwd);
        char inlin[256];
        fgets(inlin, 256, stdin);
        inlin[strlen(inlin) - 1] = 0;
        char **buff = parse_args(inlin);
    /*
    int i = 0;
    while(buff[i]){
        printf("'%d'\n", buff[i++]);
    }
    */
        int status = 0;
        int f = fork();
        wait(&status);
        char * path;
        if(!f){
            if(!strcmp(buff[0], "cd")){
                path = buff[1];
                chdir(path);
            }else{
                execvp(buff[0], buff);
            }
        }else{
            printf("PARENT STUFF\n");
        }
        free(buff);
    }
    return 0;

}
