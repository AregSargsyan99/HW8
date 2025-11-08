#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(void){
char a[256],b[256];
printf("First file path: ");
if(scanf("%255s",a)!=1)
return 1;
printf("Second file path: ");
if(scanf("%255s",b)!=1)
return 1;
int f1=open(a,O_RDONLY);
if(f1<0){
perror("open");
return 1;
}
int f2=open(b,O_RDONLY);
if(f2<0){
perror("open");
close(f1);
return 1;
}
unsigned char x[4096],y[4096];
off_t i=0;
for(;;){
ssize_t n1=read(f1,x,sizeof(x));
if(n1<0){
perror("read");
close(f1);
close(f2);
return 1;
}
ssize_t n2=read(f2,y,sizeof(y));
if(n2<0){
perror("read");
close(f1);
close(f2);
return 1;
}
if(n1==0&&n2==0)
break;
ssize_t m=n1<n2?n1:n2;
for(ssize_t j=0;j<m;j++){
if(x[j]!=y[j]){
printf("Files differ at byte %lld\n",(long long)(i+j));
close(f1);
close(f2);
return 1;
}
}
i+=m;
if(n1!=n2){
printf("Files differ at byte %lld\n",(long long)i);
close(f1);
close(f2);
return 1;
}
}
printf("Files are identical.\n");
if(close(f1)<0){
perror("close");
close(f2);
return 1;
}
if(close(f2)<0){
perror("close");
return 1;
}
return 0;
}
