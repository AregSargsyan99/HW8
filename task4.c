#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(void){
int fd=open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0644);
if(fd<0){
perror("open");
return 1;
}
char line[1024];
ssize_t n=read(STDIN_FILENO,line,sizeof(line)-1);
if(n<0){
perror("read");
close(fd);
return 1;
}
if(n==0){
if(close(fd)<0)perror("close");
return 0;
}
pid_t pid=getpid();
char pfx[64];
int plen=snprintf(pfx,sizeof(pfx),"PID=%ld: ",(long)pid);
if(plen<0){
close(fd);
return 1;
}
ssize_t o=0;
while(o<plen){
ssize_t w=write(fd,pfx+o,plen-o);
if(w<0){
perror("write");
close(fd);
return 1;
}
o+=w;
}
o=0;
while(o<n){
ssize_t w=write(fd,line+o,n-o);
if(w<0){
perror("write");
close(fd);
return 1;
}
o+=w;
}
if(line[n-1]!='\n'){
if(write(fd,"\n",1)<0){
perror("write");
close(fd);
return 1;
}
}
off_t pos=lseek(fd,0,SEEK_CUR);
if(pos<0){
perror("lseek");
close(fd);
return 1;
}
/*SEEK_CUR still grows with O_APPEND because
the file position is advanced after each
write even though writes are forced to the end of the file.*/
printf("Final offset: %lld\n",(long long)pos);
if(close(fd)<0){
perror("close");
return 1;
}
return 0;
}
