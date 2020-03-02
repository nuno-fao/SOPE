//FOLHA 2 - p4.c
//FILE COPY
//USAGE: stuf...

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_NAME_LEN 512

int main(int argc, char *argv[])
{
 int fd1;

 fd1 = open("alunosEnotas.txt",WRONLY | O_CREAT | O_TRUNC);
 if(fd1<0){
  return 1;
 }
 while (i < MAX_NAME_LEN && read(STDIN_FILENO, &ch, 1) && ch != '\n') {
  pass[i++] = ch;
  write(STDOUT_FILENO, &echo, 1);
 }
}