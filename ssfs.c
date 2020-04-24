#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <limits.h>
#include <time.h>

#define key 10
#define caesar 94

char *dir = "/home/devi/Documents";
char domain[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";
char codomain[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";

// yg dibawah ini untuk encrypt yang di comment 
// char caesar_key[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";

void generate_codomain(int keys) {
	char potongan[keys+1];
	strncpy(potongan, domain, keys);
	potongan[keys] = 0;
	sprintf(codomain, "%s%s", domain+keys, potongan);
}

void encrypt_c(char *str) {
	int index = 0;
	
	while(str[index] != '\0') {
		if(str[index] != '/') {
			int idx_domain = strchr(domain, str[index]) - domain;
			str[index] = codomain[idx_domain];
		}
		index++;	
	}
}

void decrypt_c(char *str) {
	int index = 0;
	
	while(str[index] != '\0') {
		if(str[index] != '/') {
			int idx_codomain = strchr(codomain, str[index]) - codomain;
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
			check_ext = strrchr(filetok, '.');
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
			check_ext = strrchr(filetok, '.');
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

char *LOG = "/home/devi/fs.log";

void log_warning(char* desc) {
	FILE *file_log = fopen(LOG, "a");

	time_t rawtime = time(NULL);
	struct tm tm = *localtime(&rawtime);

	int tahun = tm.tm_year+1900;
	int bulan = tm.tm_mon+1;
	int hari = tm.tm_mday;
	int jam = tm.tm_hour;
	int menit = tm.tm_min;
	int detik = tm.tm_sec;

	fprintf(file_log, "[WARNING]::[%02d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]\n", 
		tahun, bulan, hari, jam, menit, detik, desc);
	fclose(file_log);
}

void log_info(char* desc) {
	FILE *file_log = fopen(LOG, "a");

	time_t rawtime = time(NULL);
	struct tm tm = *localtime(&rawtime);

	int tahun = tm.tm_year+1900;
	int bulan = tm.tm_mon+1;
	int hari = tm.tm_mday;
	int jam = tm.tm_hour;
	int menit = tm.tm_min;
	int detik = tm.tm_sec;

	fprintf(file_log, "[INFO]::[%02d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]\n", 
		tahun, bulan, hari, jam, menit, detik, desc);
	fclose(file_log);
}

//ngebaca file
static int xmp_getattr(const char *path, struct stat *stbuf) {
    	log_info("LS");
	int res;

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi) {
    	log_info("CD");
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(path);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    	log_info("CAT");
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode) {
   	log_info("MKDIR");
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev) {
    	log_info("CREATE");
	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

//delete data
static int xmp_unlink(const char *path) {
    	log_warning("REMOVE");
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

//delete direktori
static int xmp_rmdir(const char *path) {
    log_warning("RMDIR");
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to) {
    log_info("MOVE");
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size) {
    log_info("TRUNCATE");
	int res;

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) {
    log_info("OPEN");
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi) {
    	log_info("WRITE");
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.mknod = xmp_mknod,
	.unlink = xmp_unlink,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.truncate = xmp_truncate,
	.open = xmp_open,
	.read = xmp_read,
	.write = xmp_write,
};

int main(int argc, char *argv[]) {
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
