#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <string.h>
#include <stdio.h>

#include "copylib.h"

int main (int argc, char *argv[]) {

	if (argc != 3) { // checking the number of function arguments
		printf("Usage: %s <source> <destination>\n", argv[0]);
		return ERR_ARG;
	}

	struct stat sbet; // creating a structure to get information from lstat

	if (lstat(argv[1], &sbet) == -1) { // checking lstat completion status
		
		perror("Failed to retrieve stat about the file");
		return ERR_FILE_STAT;
	}

	if (copy_file((sbet.st_mode & S_IFMT), argv[1], argv[2], &sbet) != 0) {
		perror("Failed to copy file");
		return ERR_CPY;
	}

	return 0;
}