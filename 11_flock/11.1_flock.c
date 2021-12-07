#include "../Headers/err_handle.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main (void) {
	const char* file_path = "ACCESS_COUNTER.txt"; /* name of file which contains number of program launches */
	int fd = 0;                                   /* file descriptor for opening aforementioned file */
	FILE * fp;

	fd = open(file_path, O_RDWR | O_CREAT, 0777); /* opening a counter-file */
	if (fd == -1)																	/* checking for errors    */
		handle_error("open");

	fp = fdopen (fd, "w+"); /* creating file ptr form descriptor */
	if (fp == NULL)         /* checking for errors               */
		handle_error("fdopen");


	if (lockf(fd, F_LOCK, 0) < 0) /* calling lockf to lock a section(fd) for exclusive use; */
		handle_error("lockf");			/* flag 0 means the section extends from the current file position to infinity */

	long int number_from_file = 0;
	if (fscanf(fp, "%ld", &number_from_file) < 1 && errno != 0) { /* getting value from counter file */
		int errno_code = errno; /* in case of error */
		perror("fscanf");
		lockf(fd, F_ULOCK, 0);  /* unlocking section */
		close(fd);              /* closing descriptor */
		return errno_code;
	}

	sleep(3);

	++number_from_file;
	printf("The number of times the program has been launched: %ld\n", number_from_file);

	rewind(fp); /* setting file ptr to the beginning */

	if (fprintf(fp, "%ld\n", number_from_file) < 0) { /* printing value in counter file */
		int errno_code = errno;  /* in case of error */
		perror("fprintf");
		lockf(fd, F_ULOCK, 0);   /* unlocking section */
		close(fd);               /* closing descriptor */
		return errno_code;
	}

	fflush(fp);                /* flushing output file stream */


	/* in the end of program work */
	lockf(fd, F_ULOCK, 0);     /* unlocking section */
	close(fd);								 /* closing descriptor */

	return 0;
}