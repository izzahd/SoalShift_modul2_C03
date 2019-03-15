# SoalShift_modul2_C03

## Soal 1

Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.

### Jawaban :
```
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
```
* Buka direktori tempat file png berada.
* Mencari dan menyimpan nama file pada direktori tersebut yang memiliki ekstensi .png 
* Nama file baru ditambahkan dengan _grey.png setelah nama file lama dihapus .png nya
* Menambahkan destination path pada nama file baru, juga menambahkan source path pada nama file lama
* Rename source menjadi dest agar file png berpindah ke direktori tujuan dengan penambahan _grey pada nama file nya.


## Soal 2

Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

### Jawaban :
```
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
```
* Membuat direktori hatiku dan membuat file elen.ku dengan menggunakan touch. 
* Mencari tahu owner dan group dari file elen.ku.
* Membuat struct dan menyimpan user id owner dan group di dalam struct tersebut.
* Membandingkan user id owner dan user id group www-data. Jika sama, maka file elen.ku akan dihapus. 
* Untuk mengubah permission menjadi 777 (dalam oktal) maka kita perlu mengubah string "0777" ke dalam oktal menggunakan strtol. 
* Selanjutnya, chmod.


## Soal 3

Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
i)  mengekstrak file zip tersebut.
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
Catatan:  
* Gunakan fork dan exec.
* Gunakan minimal 3 proses yang diakhiri dengan exec.
* Gunakan pipe
* Pastikan file daftar.txt dapat diakses dari text editor

### Jawaban :
```
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
```
* Buat file daftar.txt menggunakan touch.
* Unzip file campur2.zip
* Melihat file apa saja yang berada dalam folder campur2 menggunakan ls.
* Mendapatkan file-file yang berekstensi .txt menggunakan grep.
* Print hasil grep ke dalam file daftar.txt


## Soal 4

Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 
-	dilarang menggunakan crontab
-	Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

### Jawaban :
```
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
```
* Mengambil waktu akses dari file. 
* Mengambil waktu dari sistem kita. 
* Setelah mendapatkan keduanya, bandingkan selisih waktu keduanya. Jika selisihnya <= 30, maka buat sebuah file baru dengan nama makan_sehat$.txt . 
* $ merupakan iterasi sampai tak hingga. 


## Soal 5

Kerjakan poin a dan b di bawah:
a.)	Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
Ket:
-	Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
-	Per menit memasukkan log#.log ke dalam folder tersebut
‘#’ : increment per menit. Mulai dari 1
b.)	Buatlah program c untuk menghentikan program di atas.
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

### Jawaban :
```

```
Program yg menghentikan program soal5.c
```
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main() {
	char *kill_argv[]={"pkill", "soal5", NULL};
	execv("/usr/bin/pkill",kill_argv);
}
```
* Kesulitan : tidak cukup waktu dan belum mengerti


