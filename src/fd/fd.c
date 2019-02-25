#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int fd = 0;

void terminate() {
	close(fd);
}

int main() {
	fd = open("/etc/passwd", O_RDWR);
	signal(SIGUSR1, terminate);
	pause();
	return 0;
}
