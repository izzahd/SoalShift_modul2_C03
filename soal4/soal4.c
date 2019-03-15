#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

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
  	int i=1;
  	while(1) {
        	struct stat sts;
    		struct  tm timesys;
    		struct tm timeacc;
    		char loc[100];
    		char loc2[100];
    		memset(loc, 0, sizeof(loc));
    		memset(loc2, 0, sizeof(loc2));
    		strcpy(loc, "/home/boy/Documents/makanan/makan_enak.txt");
    		strcpy(loc2, "/home/boy/Documents/makanan/");
   		stat (loc,&sts);
    		timeacc = *localtime(&sts.st_atime);

    		time_t T= time(NULL);

    		timesys = *localtime(&T);
    		if (difftime(mktime(&timesys), mktime(&timeacc)) <= 30){
           		strcpy(loc2+strlen(loc2),"makan_sehat");
           		char ctr[1000];
           		sprintf(ctr,"%d", i);
           		strcpy(loc2+strlen(loc2), ctr);
           		strcpy(loc2+strlen(loc2), ".txt");
           		creat(loc2, S_IRWXU | S_IRWXG | S_IRWXO );
           		i++;
       		}
     		sleep(5);
	}
  	exit(EXIT_SUCCESS);
}
