#include <stdio.h>
#include <unistd.h>

int main() {
  int p = fork();
  if (p > 0) {
    pause();
  }
  return 0;
}
