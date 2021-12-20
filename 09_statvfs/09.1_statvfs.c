#include "../Headers/err_handle.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

int main (int argc, char* argv[]) {

	const char * filesystem_path = argc > 1 ? argv[1] : "."; /* getting directory path from function arguments; local dir default */

	struct statvfs st_vfs; /* creating struct for statvfs call */

	if (statvfs(filesystem_path, &st_vfs) < 0) /* statvfs call with err handle */
		handle_error("statvfs");

	printf("File system lock size:                      %lu\n", st_vfs.f_bsize);  /* printing fs info */
	printf("Fragment size:                              %lu\n", st_vfs.f_frsize);
	printf("Size of filesystem in fragment size units:  %lu\n", (uintmax_t)st_vfs.f_blocks);
	printf("Amount of free blocks:                      %lu\n", (uintmax_t)st_vfs.f_bfree);
	printf("Amount of free blocks for non-root:         %lu\n", (uintmax_t)st_vfs.f_bavail);
	printf("Amount of inodes:                           %lu\n", (uintmax_t)st_vfs.f_files);
	printf("Amount of free inodes:                      %lu\n", (uintmax_t)st_vfs.f_ffree);
	printf("Amount of free inodes for non-root:         %lu\n", (uintmax_t)st_vfs.f_favail);
	printf("File system ID:                             %lu\n", st_vfs.f_fsid);
	printf("Mount flags:                                %lu\n", st_vfs.f_flag);
	printf("Maximum filename length:                    %lu\n", st_vfs.f_namemax);

	if (argc == 3) { /* checking file fit if third argument exists */
		const char * file_path = argv[2]; /* getting file path from main args */
		struct stat sb; /* creating struct for lstat call */

		if (lstat(file_path, &sb) < 0) { /* lstat call with err handle */
			handle_error("lstat");
		}

	short res = 1; /* creating variable  which contains result of program work */

	if ((st_vfs.f_favail < 1) || (((unsigned long)st_vfs.f_bavail * st_vfs.f_bsize) < sb.st_size)) /* checking if there is inode numbers or        */
		res = 0;                                                                                /* if there is enough space for this file in fs */

	printf("Does the file fit:                          %s\n", res ? "Yes" : "No"); /* printing results */

	}

	return 0;
}