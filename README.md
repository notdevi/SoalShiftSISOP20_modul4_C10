# SoalShiftSISOP20_modul4_C10
Praktikum Modul 4 Sistem Operasi 2020

### Nama Anggota Kelompok :
### 1. Devi Hainun Pasya (05111840000014)
### 2. Anggara Yuda Pratama (05111840000008)

Jasir menemukan 2 buah metode enkripsi file. Pada mode enkripsi pertama, nama file-file pada direktori terenkripsi akan dienkripsi menggunakan metode substitusi. Sedangkan pada mode enkripsi kedua, file-file pada direktori terenkripsi akan di split menjadi file-file kecil.

Untuk segi efisiensi, dikarenakan pada perusahaan tersebut sering dilaksanakan sinkronisasi antara 2 direktori, maka jasir telah merumuskan sebuah metode agar filesystem-nya mampu mengsingkronkan kedua direktori tersebut secara otomatis. Agar integritas filesystem tersebut lebih terjamin, maka setiap command yang dilakukan akan dicatat kedalam sebuah file log.

- filesystem berfungsi normal layaknya linux pada umumnya.
- mount source (root) filesystem adalah direktori (/home/[user]/Documents)

### Soal No. 1

ENKRIPSI VERSI 1 :
- Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
- Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
- Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip.
- Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
- Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key.
```
9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO
```
Misal kan ada file bernama “kelincilucu.jpg” dalam directory FOTO_PENTING, dan key yang dipakai adalah 10

**“encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg"**

**Note :** Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt

- Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.

### Soal No. 2

ENKRIPSI VERSI 2 :
- Jika sebuah direktori dibuat dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.
- Jika sebuah direktori di-rename dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.
- Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi direktori tersebut akan terdekrip.
- Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
- Pada enkripsi v2, file-file pada direktori asli akan menjadi bagian-bagian kecil sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem rancangan jasir. Sebagai contoh, file File_Contoh.txt berukuran 5 kB pada direktori asli akan menjadi 5 file kecil yakni: File_Contoh.txt.000, File_Contoh.txt.001, File_Contoh.txt.002, File_Contoh.txt.003, dan File_Contoh.txt.004.
- Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lain yang ada didalam direktori tersebut (rekursif).

### Soal No. 3

Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan `sync_` yaitu `sync_dir`. Persyaratan untuk sinkronisasi yaitu :
- Kedua directory memiliki parent directory yang sama.
- Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi:
    - Nama dari setiap berkas di dalamnya sama.
    - Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik.
- Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja.
- Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory.

Jika persyaratan di atas terlanggar, maka kedua directory tersebut tidak akan tersinkronisasi lagi.

### Soal No. 4

Log System :
- Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
- Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.
- Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
- Sisanya, akan dicatat dengan level INFO.
- Format untuk logging yaitu:

```
[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]
```

LEVEL    : Level logging
yy   	 : Tahun dua digit
mm    	 : Bulan dua digit
dd    	 : Hari dua digit
HH    	 : Jam dua digit
MM    	 : Menit dua digit
SS    	 : Detik dua digit
CMD     	 : System call yang terpanggil
DESC      : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::)

Contoh format logging nantinya seperti:
```
INFO::200419-18:29:28::MKDIR::/iz1
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg
```

***ssfs.c*** : [kodingan](https://github.com/notdevi/SoalShiftSISOP20_modul4_C10/blob/master/ssfs.c)

**PENYELESAIAN :**

**No. 1**

Hal pertama dilakukan adalah mendeclare makro untuk deklarasi key yang akan dipakai dalam proses enkripsi `#define key 10`. Juga dilakukan deklarasi untuk path dari direktori Documents `static const char *dirpath = "/home/devi/Documents";`.

Kemudian key untuk proses eknkripsi dengan caesar cipher distore di dalam array `cipher`.
```c
char cipher[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
```

Proses enkripsi untuk VERSI 1 dilakukan dengan cara yaitu pertama, full path dari direktori yang akan di enkripsi akan di cek per `char` dari belakang, apabila `char` yang sedang dicek merupakan '/' akan di break. Untuk menghandle apabila file memiliki ekstensi, apabila terdapat '.' maka `str_length` akan diubah menjadi `i-1`, sehingga yang ter enkrip hanya nama file dan tanpa ekstensi.
```c
void encrypt_v1(char *str) {
	int str_length = strlen(str);
	int begin = 0, idx;
	char *ptr;

	for(int i = strlen(str); i >= 0; i--) {
		if(str[i] == '/') {
			break;
		}
		else if(str[i] == '.') {
			str_length = i-1;
		}
	}

	for(int i = 1; i < str_length; i++) {
		if(str[i] == '/') {
			begin = i;
		}
	}
```

Untuk mengambil file atau direktori paling belakang pada path, dilakukan loop dimana setiap bertemu '/', variable `begin` akan di set menjadi indeks dimana '/' tersebut berada.
```c
	for(int i = 1; i < str_length; i++) {
		if(str[i] == '/') {
			begin = i;
		}
	}
```

Kemudian dilakukan pemetaan tiap-tiap karakter sebagai hasil dari enkripsi dengan loop.
```c
	for(int i = begin; i < str_length; i++) {
		if(str[i] == '/') {
			continue;
		}
		ptr = strchr(cipher, str[i]);
		if(ptr) {
			idx = ptr - cipher;
			str[i] = cipher[(idx + key) % 87];
		}
	}
}
```

Proses dekripsi untuk VERSI 1 dilakukan dengan cara yaitu pertama, full path akan dicek per char dari depan, apabila ditemukan '/' atau end of array `\0`, maka variable `begin` di set menjadi `i + 1` lalu di break.
```c
void decrypt_v1(char *str) {
	int str_length = strlen(str);
	int begin = 0, idx;
	char *ptr;

	for(int i = 1; i < str_length; i++) {
		if(str[i] == '/' || str[i+1] == '\0') {
			begin = i+1;
			break;
		}
	}
```

Kemudian dilakukan loop untuk mengidentifikasi apakah file memiliki ekstensi atau tidak.
```c
	for(int i = strlen(str); i >= 0; i--) {
		if(str[i] == '/') {
			break;
		}
		else if(str[i] == '.' && i == (strlen(str)-1)) {
			str_length = strlen(str);
			break;
		}
		else if(str[i] == '.' && i != (strlen(str)-1)) {
			str_length = i-1;
			break;
		}
	}
```

Kemudian dilakukan pemetaan tiap-tiap karakter sebagai hasil dari enkripsi dengan loop.
```c
	for(int i = begin; i < str_length; i++) {
		if(str[i] == '/') {
			continue;
		}
		ptr = strchr(cipher, str[i]);
		if(ptr) {
			idx = ptr - cipher - key;
			if(idx < 0) {
				idx += 87;
			}
			str[i] = cipher[idx];
		}
	}
}
```

Fungsi encrypt decrypt diatas nanti dipanggil di fungsi-fungsi fuse yang ada. Untuk encrypt hanya dipanggil pada `xmp_readdir`:
```c

.....

	DIR *dp;
   	struct dirent *de;

    	(void) offset;
    	(void) fi;

    	dp = opendir(fpath);

    	if(dp == NULL) return -errno;

   	while ((de = readdir(dp)) != NULL) {
        	struct stat st;
	        memset(&st, 0, sizeof(st));
        	st.st_ino = de->d_ino;
        	st.st_mode = de->d_type << 12;
		//titik sama dua titik direktori
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
			continue;
		}

		char temp[1000];
		
		strcpy(temp, de->d_name);	// ngambil nama file doang
		// printf("ini temp = %s\n", temp);

		if(strncmp(path, "/encv1_", 7) == 0) {
			encrypt_v1(temp);
		}
		res = (filler(buf, temp, &st, 0));

        	if (res != 0) break;
    	}
```

Sedangkan untuk fungsi decrypt dipanggil pada `xmp_getattr`, `xmp_readdir`, `xmp_read`, `xmp_truncate`, `xmp_write`, `xmp_unlink`, `xmp_rename`, `xmp_mkdir`, `xmp_rmdir`, `xmp_open` serta `xmp_mknod` yang jika ada nama folder bernama "encv1_" maka fungsi tersebut dipanggil :
```c
	if (strcmp(path, "/") == 0) {
        	path = dirpath;
        	sprintf(fpath, "%s", path);
    	} else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt_v1(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}
```

**No. 2 dan 3**

Belum berhasil diselesaikan.

**No. 4**

Untuk menghandle pembuatan file histori `fs.log`, dilakukan dalam dua fungsi yaitu `void log_warning(char* desc, char* path)` dan  `void log_info(char* desc, char* path)`, dibedakan berdasarkan jenis level.

Pertama-tama path file yang telah di declare dalam variable `LOG` akan di open dan dilakukan append 
```c
FILE *file_log = fopen(LOG, "a");
```

Kemudian dilakukan pengambilan local time dengan `struct tm`, lalu local time akan disimpan pada file `file_log` sesuai format.
```c
    time_t rawtime = time(NULL);
	struct tm tm = *localtime(&rawtime);

	int tahun = tm.tm_year+1900;
	int bulan = tm.tm_mon+1;
	int hari = tm.tm_mday;
	int jam = tm.tm_hour;
	int menit = tm.tm_min;
	int detik = tm.tm_sec;

	fprintf(file_log, "WARNING::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n", 
		tahun, bulan, hari, jam, menit, detik, desc, path);
	fclose(file_log);
```

Berlaku juga pada fungsi `log_info`.
```c
void log_info(char* desc, char* path) {
	FILE *file_log = fopen(LOG, "a");

	time_t rawtime = time(NULL);
	struct tm tm = *localtime(&rawtime);

	int tahun = tm.tm_year+1900;
	int bulan = tm.tm_mon+1;
	int hari = tm.tm_mday;
	int jam = tm.tm_hour;
	int menit = tm.tm_min;
	int detik = tm.tm_sec;

	fprintf(file_log, "INFO::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n", 
		tahun, bulan, hari, jam, menit, detik, desc, path);
	fclose(file_log);
}
```

Kedua fungsi diatas akan dipanggil pada masing-masing fungsi fuse sesuai dengan kebutuhan yang diperlukan, semisal :
```c
	log_info("MKDIR", fpath);
```

dan juga untuk log_warning:
```c
	log_warning("RMDIR", fpath);
```
