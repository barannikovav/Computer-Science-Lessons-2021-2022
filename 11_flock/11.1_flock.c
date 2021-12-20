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
  if (fd == -1) {                               /* checking for errors    */
    
    perror("open");

  } else {
  	
  	fp = fdopen (fd, "r+"); /* creating file ptr form descriptor */

  	if (fp == NULL) {       /* checking for errors               */
  		
  		perror("fdopen");

  	} else {
  		
  		if (lockf(fd, F_LOCK, 0) < 0) { /* calling lockf to lock a section(fd) for exclusive use; */
  			perror("lockf")               /* flag 0 means the section extends from the current file position to infinity */
  		} else {

  			unsigned long number_from_file = 0;

  			if (fscanf(fp, "%lu", &number_from_file) < 1 && errno != 0) { /* getting value from counter file */
  				perror("fsacnf");
  			} else {

  				sleep(3);

  				++number_from_file;
  				printf("The number of times the program has been launched: %ld\n", number_from_file);

  				rewind(fp); /* setting file ptr to the beginning */

  				if (fprintf(fp, "%ld\n", number_from_file) < 0) {
  					perror("fprintf");
  				} else {

  					fflush(fp); /* flushing output file stream */

  				}

  			}

  		}

  	}
  }

  /* in the end of program work */
  lockf(fd, F_ULOCK, 0);     /* unlocking section */
  fclose(fp);                /* closing descriptor */

  return 0;
}