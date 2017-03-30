#include <stdio.h>
#include <unistd.h>

int main() {
  //write("tty: %s\n", ttyname(0));
  printf("%s", ttyname(0));
  dprintf(3, "%s", ttyname(0));
  return 0;
}
