#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
int main(void){
int fd;
const char*txt="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
size_t len=strlen(txt);
ssize_t n;
fd=open("data.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
if(fd<0){
perror("open create");
return 1;
}
size_t off=0;
while(off<len){
n=write(fd,txt+off,len-off);
if(n<0){
perror("write");
close(fd);
return 1;
}
off+=n;
}
if(close(fd)<0){
perror("close");
return 1;
}
fd=open("data.txt",O_RDWR);
if(fd<0){
perror("open rdwr");
return 1;
}
off_t sz=lseek(fd,0,SEEK_END);
if(sz<0){
perror("lseek");
close(fd);
return 1;
}
printf("Size before truncate: %lld\n",(long long)sz);
if(truncate("data.txt",10)<0){
perror("truncate");
close(fd);
return 1;
}
sz=lseek(fd,0,SEEK_END);
if(sz<0){
perror("lseek");
close(fd);
return 1;
}
printf("Size after truncate: %lld\n",(long long)sz);
if(lseek(fd,0,SEEK_SET)<0){
perror("lseek");
close(fd);
return 1;
}
char buf[64];
while((n=read(fd,buf,sizeof(buf)))>0){
ssize_t o=0;
while(o<n){
ssize_t w=write(STDOUT_FILENO,buf+o,n-o);
if(w<0){
perror("write");
close(fd);
return 1;
}
o+=w;
}
}
if(n<0){
perror("read");
close(fd);
return 1;
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
