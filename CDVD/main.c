#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <loadfile.h>
#include <sifrpc.h>
#include <iopcontrol.h>
#include <sbv_patches.h>
#include <dirent.h>
#include <unistd.h>

extern unsigned char cdvd_irx;
extern unsigned int size_cdvd_irx;

int main()
{      
	int fd;
	DIR *dirp, *dirp2;
	struct dirent *direntp;
	struct stat buf;

	while(!SifIopReset(NULL, 0)){};
	while(!SifIopSync()){};
	SifInitRpc(0);
	sbv_patch_enable_lmb();

	/* CDVD */
	int loaded = SifExecModuleBuffer(&cdvd_irx, size_cdvd_irx, 0, NULL, NULL);
	printf("IRX has been loaded %i\n", loaded);

	// I/O functions to try
	printf("*****\n\nTrying mkdir\n *****\n\n");
	mkdir("cdfs:test", 0755);

	printf("*****\n\nTrying open\n *****\n\n");
	fd = open("cdfs:README.TXT", O_RDONLY);

	printf("*****\n\nTrying close\n *****\n\n");
	close(fd);

	printf("*****\n\nTrying open\n *****\n\n");
	fd = open("cdfs:BADAPPLE4.ZIP", O_RDONLY);

	printf("*****\n\nTrying close\n *****\n\n");
	close(fd);

	printf("*****\n\nTrying open\n *****\n\n");
	fd = open("cdfs:PS2IDENT", O_RDONLY);

	printf("*****\n\nTrying close\n *****\n\n");
	close(fd);

	printf("*****\n\nTrying opendir\n *****\n\n");
	dirp = opendir("cdfs:");
	if (dirp == NULL) {
		printf("Error dir can not be opened\n");
	}

	dirp2 = opendir("cdfs:GB");
	if (dirp2 == NULL) {
		printf("Error dir can not be opened\n");
	}

	while ((direntp = readdir(dirp)) != NULL) {
		printf("Reading dir!\n");
	}

	printf("*****\n\nTrying getStat\n *****\n\n");
	stat("cdfs:/GB", &buf);

	printf("Is dir %i\n", S_ISDIR(buf.st_mode));

	while (1) {
		// if (index == 1000000000) {
		// 	printf("Alive!\n");
		// 	index = 0;
		// } else {
		// 	index++;
		// }
	}

	return 0;
}
