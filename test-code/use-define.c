#include<stdio.h>

#define NAME "Stephen Strange"
#define YEAR 2016

int main() {
    printf("Using #define...\n");
    printf("%s had a movie in %d\n",NAME,YEAR);
    printf("%s had a movie in %d too\n",NAME,YEAR+6);
    return 0;
}