/*
#include <unistd.h>

int access(const char *path, int amode);

*/

#include <unistd.h>
#include <stdio.h>

int main(){
    int res=access("a.txt",X_OK);
    if(res==-1){
        perror("access");
        return -1;
    }
    printf("yes!\n");
    return 0;
}