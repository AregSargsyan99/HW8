#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(void){
int fd=open("sparse.bin",O_WRONLY|O_CREAT|O_TRUNC,0644);
if(fd<0){
perror("open");
return 1;
}
const char*s1="START",*s2="END";
ssize_t n;
size_t o=0;
while(o<5){
n=write(fd,s1+o,5-o);
if(n<0){
perror("write");
close(fd);
return 1;
}
o+=n;
}
if(lseek(fd,1024*1024,SEEK_CUR)<0){
perror("lseek");
close(fd);
return 1;
}
o=0;
while(o<3){
n=write(fd,s2+o,3-o);
if(n<0){
perror("write");
close(fd);
return 1;
}
o+=n;
}
if(close(fd)<0){
perror("close");
return 1;
}
fd=open("sparse.bin",O_RDONLY);
if(fd<0){
perror("open");
return 1;
}
off_t sz=lseek(fd,0,SEEK_END);
if(sz<0){
perror("lseek");
close(fd);
return 1;
}
printf("Apparent file size: %lld\n",(long long)sz);
/*Disk usage is much smaller because the skipped region is 
a sparse hole: the filesystem does not allocate  the 
blocks for unwritten zero bytes.*/
if(close(fd)<0){
perror("close");
return 1;
}
return 0;
}
