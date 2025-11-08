#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(void){
int fd=open("numbers.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
if(fd<0){
perror("open");
return 1;
}
char buf[32];
for(int i=1;i<=10;i++){
int l=snprintf(buf,sizeof(buf),"%d\n",i);
if(l<0){close(fd);return 1;}
int o=0;
while(o<l){
ssize_t w=write(fd,buf+o,l-o);
if(w<0){perror("write");close(fd);
return 1;}
o+=w;
}
}
if(close(fd)<0){perror("close");
return 1;}
fd=open("numbers.txt",O_RDWR);
if(fd<0){perror("open");
return 1;}
off_t pos=0,start=-1,end=-1;
int ln=1;char c;ssize_t n;
while((n=read(fd,&c,1))==1){
if(ln==4&&start<0)start=pos;
pos++;
if(c=='\n'){if(ln==4){end=pos;break;}
ln++;}
}
if(n<0){perror("read");close(fd);return 1;}
if(start<0||end<0){fprintf(stderr,"line 4 not found\n");
close(fd);return 1;}
off_t endpos=lseek(fd,0,SEEK_END);
if(endpos<0){
perror("lseek");
close(fd);
return 1;
}
off_t tail=endpos-end;
char*t=NULL;
if(tail>0){
t=malloc(tail);
if(!t){
perror("malloc");
close(fd);
return 1;
}
if(lseek(fd,end,SEEK_SET)<0){
perror("lseek");
free(t);close(fd);
return 1;
}
off_t rcv=0;
while(rcv<tail){
n=read(fd,t+rcv,tail-rcv);
if(n<0){perror("read");
free(t);
close(fd);
return 1;
}
if(n==0)
break;
rcv+=n;
}
}
if(lseek(fd,start,SEEK_SET)<0){
perror("lseek");
free(t);
close(fd);
return 1;
}
const char*r="100\n";size_t rl=4,wr=0;
while(wr<rl){
n=write(fd,r+wr,rl-wr);
if(n<0){
perror("write");
free(t);
close(fd);
return 1;
}
wr+=n;
}
if(tail>0){
off_t s=0;
while(s<tail){
n=write(fd,t+s,tail-s);
if(n<0){
perror("write");
free(t);
close(fd);
return 1;}
s+=n;
}
free(t);
}
if(close(fd)<0){
perror("close");
return 1;
}
fd=open("numbers.txt",O_RDONLY);
if(fd<0){
perror("open");
return 1;
}
while((n=read(fd,buf,sizeof(buf)))>0){
ssize_t o=0;
while(o<n){
ssize_t w=write(STDOUT_FILENO,buf+o,n-o);
if(w<0){
perror("write");
close(fd);
return 1;}
o+=w;
}
}
if(n<0){
perror("read");
close(fd);
return 1;
}
if(close(fd)<0){
perror("close");
return 1;}
return 0;
}
