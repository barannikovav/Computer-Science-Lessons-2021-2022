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
	int counter = 1;                              /* counter of program launches */

	char buffer[sizeof("9999")] = {0};                  /* maximum amount of openings - 9999  */

	fd = open(file_path, O_RDWR | O_CREAT, 0777); /* opening a counter-file */
	if (fd == -1)																	/* checking for errors    */
		handle_error("open");

	if (lockf(fd, F_LOCK, 0) < 0) /* calling lockf to lock a section(fd) for exclusive use; */
		handle_error("lockf");			 /* flag 0 means the section extends from the current file position to infinity */

	long int read_output = read(fd, buffer, sizeof(buffer)); /* calling read to get number from file */

	if (read_output < 0) {       /* in case of error */
		int errno_code = errno;
		perror("read");
		lockf(fd, F_ULOCK, 0);     /* unlocking section */
		close(fd);                 /* closing descriptor */
		return errno_code;
	}

	sleep(3);

	counter += atoi(buffer);
	printf("The number of times the program has been launched: %d", counter);

	char new_value[sizeof("9999")] = {0};
	sprintf(new_value, "%d\n", counter);

	if (pwrite(fd, new_value, sizeof(new_value), 0) < 0) { /* in case of error */
		int errno_code = errno;
		perror("pwrite");
		lockf(fd, F_ULOCK, 0);          /* unlocking section */
		close(fd);                      /* closing descriptor */
		return errno_code;
	}

	                                /* in the end of program work */
	lockf(fd, F_ULOCK, 0);          /* unlocking section */
	close(fd);											/* closing descriptor */

	return 0;
}