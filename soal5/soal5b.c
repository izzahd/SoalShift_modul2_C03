#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main() {
	char *kill_argv[]={"pkill", "soal5", NULL};
	execv("/usr/bin/pkill",kill_argv);
}
