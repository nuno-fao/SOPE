//FOLHA 2 - p3a.c
//FILE COPY
//USAGE: copy print file's content

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
 int fd1, fd2, nr, nw;
 unsigned char buffer[BUFFER_SIZE];
 int out = STDOUT_FILENO;

 if (argc != 2 && argc != 3) {
  printf("Usage: %s <source> <destination>\n", argv[0]);
  return 1;
 }
 if(argc==3){
  fd2 = open(argv[2], O_WRONLY| O_CREAT | O_TRUNC);
  dup2(fd2,STDOUT_FILENO);
  if (fd2 == -1) {
    perror(argv[1]);
  return 2;
  }
 }
 fd1 = open(argv[1], O_RDONLY );
 if (fd1 == -1) {
  perror(argv[1]);
  return 2;
 }

 while ((nr = read(fd1, buffer, BUFFER_SIZE)) > 0){
    write(STDOUT_FILENO, buffer, nr);
  }
  if(argc==2){
    write(STDOUT_FILENO,"\n",1);
  }
  close(fd1);
  close(fd2);
  return 0;
}