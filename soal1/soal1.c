#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int main() {
	pid_t pid, sid;

	pid = fork();

  	if (pid < 0) {
    		exit(EXIT_FAILURE);
  	}

  	if (pid > 0) {
    		exit(EXIT_SUCCESS);
  	}

  	umask(0);

  	sid = setsid();

  	if (sid < 0) {
    		exit(EXIT_FAILURE);
  	}

  	if ((chdir("/")) < 0) {
    		exit(EXIT_FAILURE);
  	}

  	close(STDIN_FILENO);
  	close(STDOUT_FILENO);
  	close(STDERR_FILENO);

  	while(1) {
		char nama[5000];
        	char namapng[5000];
        	char *ilangpng;
        	char *source;
        	char *dest;

        	DIR *d;
        	struct dirent *dir;
        	d = opendir("/home/izzah/modul2");

        	if (d != NULL)
        	{
                	while ((dir = readdir(d)) != NULL)
                	{
                        	strcpy(nama, dir->d_name);
                        	strcpy(namapng, nama);
                        	ilangpng = strrchr(nama, '.');

                        	if (strstr(nama, ".png"))
                        	{
                                	*ilangpng = '\0';
                                	strcat(nama, "_grey.png");
                                	char fdest[100] = "/home/izzah/modul2/gambar/";
                                	char fsource[100]= "/home/izzah/modul2/";
                                	dest = strcat(fdest, nama);
                                	source = strcat(fsource, namapng);;
                                	rename(source, dest);
                        	}
                	}	
                	closedir(d);
        	}
		sleep(30);
  	}
  	exit(EXIT_SUCCESS);
}
