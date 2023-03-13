/*
#include <sys/types.h>
#include <dirent.h>
DIR *opendir(const char *name);
DIR *fdopendir(int fd);

DIR* :目录流

struct dirent {
               ino_t          d_ino;        //inode number 
               off_t          d_off;        //not an offset; see NOTES 
               unsigned short d_reclen;     //length of this record 
               unsigned char  d_type;       //type of file; not supported
                                              by all file system types 
               char           d_name[256];  //filename 
           };

*/




#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//读取某个目录下的所有普通文件的个数

int readFileNumber(const char* path){
    DIR* dir=opendir(path);
    if(dir==NULL){
        perror("opendir");
        exit(0);
    }
    struct dirent *ptr;
    int total=0;
    while((ptr=readdir(dir))!=NULL){
        //获取名称
        char *dname=ptr->d_name;
        //忽略.和..
        if(strcmp(dname,".")==0 ||strcmp(dname,"..")==0){
            continue;
        }
        if(ptr->d_type == DT_DIR){
            //目录，继续读取
            char newpath[256];
            sprintf(newpath,"%s/%s",path,dname);
            total+=readFileNumber(newpath);
        }

        if(ptr->d_type == DT_REG){
            total++;
        }
    }
    closedir(dir);
    return total;
}

int main(int argc,char *argv[]){
    if(argc<2){
        printf("Please input the directory!\n");
        return -1;
    }
    int n=readFileNumber(argv[1]);
    printf("普通文件个数：%d\n",n);
    return 0;
}

