#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
        pid_t child_id;
        pid_t child_id2;
        pid_t child_id3;
        pid_t child_id4;
        int status;

        child_id = fork();

        if (child_id < 0) {
                exit(EXIT_FAILURE);
        }

        if (child_id == 0) {
                char *argv[3] = {"unzip", "campur2.zip", NULL};
                execv("/usr/bin/unzip", argv);
        }
        else {
                child_id2 = fork();

                if (child_id2 < 0) {
                        exit(EXIT_FAILURE);
                }

                if (child_id2 == 0) {
                        char *argv[3] = {"touch", "daftar.txt", NULL};
                        execv("/usr/bin/touch", argv);

                }
                else {
                        while ((wait(&status))> 0);
                        FILE *out_file = fopen("daftar.txt","w");

                        char *ls[3] = {"ls", "campur2", NULL};
                        char *grep[3] = {"grep", ".*.txt$", NULL};

                        int A[2], B[2];
			                  int output;
                        char abcde[5000];

                        pipe(A);
                        pipe(B);

                        if (pipe(A) < 0){ 
                                exit(EXIT_FAILURE);
                        }

                        if (pipe(B) < 0){
                                exit(EXIT_FAILURE);
                        }

                        child_id3 = fork();

                        if (child_id3 < 0) {
                                exit(EXIT_FAILURE);
                        }

                        if (child_id3 == 0) {
                                dup2(A[1], 1);
				                        close(A[0]);
                                close(A[1]);

                                execvp ("ls", ls);
                        }
                        else {
                                child_id4 = fork ();

                                if (child_id4 == 0){
                                        dup2(A[0], 0);
                                        dup2(B[1], 1);

                                        close(A[1]);
                                        close(A[0]);
                                        close(B[1]);
                                        close(B[0]);

                                        execvp("grep", grep);
                                }
                                else {
                                        close(B[1]);
                                        close(A[1]);
					                              close(A[0]);

                                        output = read(B[0], abcde, sizeof(abcde));
                                        fprintf(out_file, "%.*s\n", output, abcde);
                                }
                        }
			                  fclose(out_file);
                }
        }
}
