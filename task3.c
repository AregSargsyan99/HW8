#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(void){
char path[256],c;
int fd;
printf("File path: ");
if(scanf("%255s",path)!=1)
return 1;
fd=open(path,O_RDONLY);
if(fd<0){
perror("open");
return 1;
}
off_t s=lseek(fd,0,SEEK_END);
if(s<0){
perror("lseek");
close(fd);
return 1;
}
while(s>0){
s--;
if(lseek(fd,s,SEEK_SET)<0){
perror("lseek");
close(fd);
return 1;
}
ssize_t n=read(fd,&c,1);
if(n<0){
perror("read");
close(fd);
return 1;
}
if(n==0)break;
if(write(STDOUT_FILENO,&c,1)<0){
perror("write");
close(fd);
return 1;
}
}
if(write(STDOUT_FILENO,"\n",1)<0){
perror("write");
close(fd);
return 1;
}
if(close(fd)<0){
perror("close");
return 1;
}
return 0;
}
