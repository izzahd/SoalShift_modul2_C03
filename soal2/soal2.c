include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>


int main() {
    	pid_t pid, sid;

  	pid = fork();

  	if (pid < 0) {
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
    	while(1){
    		struct stat ss;
    		char bro[100] = "/home/boy/hatiku/elen.ku";
		
		stat(bro, &ss);
    		struct passwd *pw = getpwuid(ss.st_uid);
    		struct group  *gr = getgrgid(ss.st_gid);

		if((strcmp(pw->pw_name, "www-data") == 0) && (strcmp(gr->gr_name, "www-data") ==0))

      		{
     			remove(bro);
      		}

      		char mode[] = "0777";
      		int i;
		i = strtol (mode, 0, 8);
      		chmod(bro,i);
     		sleep(3);
    		}
   	exit(EXIT_SUCCESS);
}
