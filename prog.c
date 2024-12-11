#include <stdio.h>

typedef enum {consomateur,station} type_Utilisateur;

int main(){
    unsigned int a, b, c;
    type_Utilisateur d = station;

    while(scanf("%d;%u;%u;%u", &d, &a, &b, &c) == 4){
        printf("a = %u\n", a);
        printf("b = %u\n", b);
        printf("c = %u\n", c);
        printf("d = %u\n", d);
        //if(scanf("%d;%u;%u;%u", &d, &a, &b, &c) != 4){
        //    fprintf(stderr, "Error reading input\n");
        //    return 1;
        //}
    }
    
    return 0;    
}
