#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(void){
char src[256],dst[256],buf[4096];
int in_fd,out_fd;
ssize_t n;
off_t total=0;
printf("Source path: ");
if(scanf("%255s",src)!=1)
return 1;
printf("Destination path: ");
if(scanf("%255s",dst)!=1)
return 1;
in_fd=open(src,O_RDONLY);
if(in_fd<0){
perror("open source");
return 1;
}
out_fd=open(dst,O_WRONLY|O_CREAT|O_TRUNC,0644);
if(out_fd<0){
perror("open dest");
close(in_fd);
return 1;
}
while((n=read(in_fd,buf,sizeof(buf)))>0){
ssize_t off=0;
while(off<n){
ssize_t w=write(out_fd,buf+off,n-off);
if(w<0){
perror("write");
close(in_fd);
close(out_fd);
return 1;
}
off+=w;
total+=w;
}
}
if(n<0){
perror("read");
close(in_fd);
close(out_fd);
return 1;
}
printf("Bytes copied: %lld\n",(long long)total);
if(close(in_fd)<0){
perror("close");
close(out_fd);
return 1;
}
if(close(out_fd)<0){
perror("close");
return 1;
}
return 0;
}
