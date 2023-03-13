/*
#include <sys/stat.h>

int chmod(const char *path, mode_t mode);
*/

#include <sys/stat.h>
#include <stdio.h>

int main(){
    int ret=chmod("a.txt",0775);
    if(ret==-1){
        perror("chmod");
        return -1;
    }
    printf("yes!\n");
    return 0;
}