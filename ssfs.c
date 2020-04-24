#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <limits.h>

#define key 10
#define caesar 94

char *DIR = "/home/devi/Documents";
char domain[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";
char codomain[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";

// yg dibawah ini untuk encrypt yang di comment 
// char caesar_key[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";

void generate_codomain(int key);
void encrypt_c(char *str);
void decrypt_c(char *str);
void encrypt_v1(char *path);
void decrypt_v1(char *path);
void warning(char* desc);
void info(char* desc);

void generate_codomain(int key) {
	char potongan[key+1];
	strncpy(potongan, domain, key);
	potongan[key] = 0;
	sprintf(codomain, "%s%s", domain+key, potongan);
}

void encrypt_c(char *str) {
	int index = 0;
	
	while(str[index] != '\0') {
		if(str[index] != '/') {
			int idx_domain = strchr(domain, str[index] - domain;
			str[index] = codomain[idx_domain];
		}
		index++;	
	}
}

void decrypt_c(char *str) {
	int index = 0;
	
	while(str[index] != '\0') {
		if(str[index] != '/') {
			int idx_codomain = strchr(codomain, str[index] - codomain;
			str[index] = domain[idx_codomain];
		}
		index++;	
	}
}

void encrypt_v1(char *path) {
	char str[100];
	char* check_ext, ext[5];
	char* ada;
	int flag;
	
	strcpy(str, path);
	char* filetok = strtok(str, "/");
	if(strcmp("encv1_rahasia", filetok) == 0) {
		sprintf(path, "%s", filetok);
		filetok = strtok(NULL, "/");
		while(filetok != NULL) {
			*check_ext = strrchr(filetok, '.');
			if(check_ext) {
				// printf("%s ada extensinya\n", filetok);
				strcpy(ext, check_ext);
				while(ada = strstr(filetok, ext)) {
					*ada = '\0';
					strcat(filetok, ada+(strlen(ext)));
				}
				flag = 0;
			} else {
				flag = 1;
				// printf(%s gaada extensinya\n", filetok);
			}
			
			if(flag == 1) {				
				encrypt_c(filetok);
				strcat(path, "/");
				strcat(path, filetok);
				filetok = strtok(NULL, "/");
			} else {
				encrypt_c(filetok);
				strcat(path, "/");
				strcat(path, filetok);
				strcat(path, ext);
				filetok = strtok(NULL, "/");
			}
		}
	}			 	
}

void decrypt_v1(char *path) {
	char str[100];
	char* check_ext, ext[5];
	char* ada;
	int flag;
	
	strcpy(str, path);
	char* filetok = strtok(str, "/");
	if(strcmp("encv1_rahasia", filetok) == 0) {
		sprintf(path, "%s", filetok);
		filetok = strtok(NULL, "/");
		while(filetok != NULL) {
			*check_ext = strrchr(filetok, '.');
			if(check_ext) {
				// printf("%s ada extensinya\n", filetok);
				strcpy(ext, check_ext);
				while(ada = strstr(filetok, ext)) {
					*ada = '\0';
					strcat(filetok, ada+(strlen(ext)));
				}
				flag = 0;
			} else {
				flag = 1;
				// printf(%s gaada extensinya\n", filetok);
			}
			
			if(flag == 1) {				
				decrypt_c(filetok);
				strcat(path, "/");
				strcat(path, filetok);
				filetok = strtok(NULL, "/");
			} else {
				decrypt_c(filetok);
				strcat(path, "/");
				strcat(path, filetok);
				strcat(path, ext);
				filetok = strtok(NULL, "/");
			}
		}
	}			 			 	
}

// encrypt yang di comment
/* 
void encrypt_c(char str[]) {
	char cur;
	
	for(int i=0; i<strlen(str); i++) {
		cur = str[i];
		for(int j=0; j<caesar+key; j++) {
			if(cur == caesar_key[j]) {
				cur = caesar_key[j+key];
				str[i] = cur;
				break;
			}
		}
	}
}

void decrypt_c(char str[]) {
	char cur;

	for(int i=(strlen(str))-1; i>=0; i--) {
		cur = str[i];
		for(int j=caesar+key-1; j>=0; j--) {
			if(cur == caesar_key[j]) {
				cur = caesar_key[j-key];
				str[i] = cur;
				break;
			}
		}
	}
}
*/

char *LOG = "/home/devi/fs.log";

void warning(char* desc) {
	FILE *file_log = fopen(LOG, "a");

	time_t rawtime = time(NULL);
	struct tm tm = *localtime(&rawtime);

	int tahun = tm.tm_year+1900;
	int bulan = tm.tm_mon+1;
	int hari = tm.tm_mday;
	int jam = tm.tm_hour;
	int menit = tm.tm_min;
	int detik = tm.tm_sec;

	sprintf(LOG, "[WARNING]::[%02d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]\n", 
		tahun, bulan, hari, jam, menit, detik, desc);
	fclose(LOG);

	return 0;
}

void info(char* desc) {
	FILE *file_log = fopen(LOG, "a");

	time_t rawtime = time(NULL);
	struct tm tm = *localtime(&rawtime);

	int tahun = tm.tm_year+1900;
	int bulan = tm.tm_mon+1;
	int hari = tm.tm_mday;
	int jam = tm.tm_hour;
	int menit = tm.tm_min;
	int detik = tm.tm_sec;

	sprintf(LOG, "[INFO]::[%02d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]\n", 
		tahun, bulan, hari, jam, menit, detik, desc);
	fclose(LOG);

	return 0;
}

int main(int argc, char *argv[]) {
	char test[] = "encv1_rahasia/FOTO_PENTING/kelincilucu.jpg";
	
	generate_codomain(10);
	encrypt_v1(test);
	puts(test);
	decrypt_v1(test);
	puts(test);
}








