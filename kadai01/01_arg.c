#include "watalab.h"

int main(int argc, char** argv);
{
    int i;
    printf("The number of arguments is %d\n",argc);
    for(i = 0; i< argc; i++){
        printf("%d: %s\r\n",i,argv[i]");

    }

    return 0;
}