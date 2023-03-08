/*
模拟实现ls-l指令
drwxr-xr-x. 3 root root  35 3月   8 18:46 gitskills
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, char* argv[]){
    if(argc<2){
        printf("Please input filename!\n");
        return -1;
    }
    struct stat info;
    int ret=stat(argv[1],&info);
    if(ret==-1){
        perror("fail to get info!\n");
        return -1;
    }
    //文件权限
    char perm[11];
    switch (info.st_mode & __S_IFMT)
    {
        case __S_IFDIR:
            perm[0]='d';
            break;
        case __S_IFBLK:
            perm[0]='b';
            break;
        case __S_IFCHR:
            perm[0]='c';
            break;
        case __S_IFIFO:
            perm[0]='i';
            break;
        case __S_IFLNK:
            perm[0]='l';
            break;
        case __S_IFREG:
            perm[0]='-';
            break;
        case __S_IFSOCK:
            perm[0]='s';
            break;
        default:
            perm[0]='?';
            break;
    }
    //文件访问权限
    perm[1]=info.st_mode & S_IRUSR?'r':'-';
    perm[2]=info.st_mode & S_IWUSR?'w':'-';
    perm[3]=info.st_mode & S_IXUSR?'x':'-';

    perm[4]=info.st_mode & S_IRGRP?'r':'-';
    perm[5]=info.st_mode & S_IWGRP?'w':'-';
    perm[6]=info.st_mode & S_IXGRP?'x':'-';

    perm[7]=info.st_mode & S_IROTH?'r':'-';
    perm[8]=info.st_mode & S_IWOTH?'w':'-';
    perm[9]=info.st_mode & S_IXOTH?'x':'-';
    perm[10]='.';
    int link_num=info.st_nlink;
    
    char* usrname=getpwuid(info.st_uid)->pw_name;
    char* grpname=getgrgid(info.st_gid)->gr_name;

    long int size=info.st_size;
    char* time=ctime(&info.st_mtime);
    char swap[512];
    strncpy(swap,time,strlen(time)-1);
    char buf[1024];
    sprintf(buf,"%s %d %s %s %d %s %s\n",perm,link_num,usrname,grpname,
            size,swap,argv[1]);
    printf("%s",buf);
    return 0;
}