//FOLHA 2 - p2b.c
//LEITURA DE PASSWORD, SEM ECOAR CARACTER LIDO

#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_PASSWD_LEN 20

int main(void)
{
 struct termios term, oldterm;
 int i;
 char pass[MAX_PASSWD_LEN+1], ch, echo = '*';

 write(STDOUT_FILENO, "\nPassword? ", 11);

 tcgetattr(STDIN_FILENO, &oldterm);
 term = oldterm;
 term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
 tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);

 i=0;
 while (i < MAX_PASSWD_LEN && read(STDIN_FILENO, &ch, 1) && ch != '\n') {
  pass[i++] = ch;
  write(STDOUT_FILENO, &echo, 1);
 }
 pass[i] = 0;

 tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
 write(STDOUT_FILENO, "\n\nPassword: ", 12);
 write(STDOUT_FILENO, pass, strlen(pass));
 write(STDOUT_FILENO, "\n", 1);

 return 0;
}

