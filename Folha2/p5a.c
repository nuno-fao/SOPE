#include ... //a completar

int main(void)
{
  int fd;
  char *text1="AAAAA";
  char *text2="BBBBB";

  fd = open("f1.txt",O_CREAT|O_EXCL|O_TRUNC|O_WRONLY|O_SYNC,0600);
  write(fd,text1,5);
  write(fd,text2,5);
  close(fd);
  return 0;
}

