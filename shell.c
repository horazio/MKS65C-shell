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
    printf("Yeah OK:\n>>");
    char inlin[256];
    fgets(inlin, 256, stdin);

    return 0;

}
