
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#include <fcntl.h> //open
#include <unistd.h> //close


//-----------------------------------------------------------------------------------------------------------------------
ssize_t pwriteall (int fd, const void *buf, size_t count, int offset) 
{
	size_t bytes_written = 0;
	const uint8_t *buf_addr = buf;
	
	while (bytes_written < count) {
		
		ssize_t res = pwrite(fd, buf_addr + bytes_written, count - bytes_written, offset);
		
		if (res < 0) {
			
			return res;

		}
		
		bytes_written += (size_t)res;
	}
	
	return (ssize_t)bytes_written;
}

//-----------------------------------------------------------------------------------------------------------------------
enum 
{

ERR_ARG,
ERR_LST,
ERR_FTY,
ERR_FOF,
ERR_FOS,
ERR_FRE,
ERR_FWR,
ERR_FFC,
ERR_SFC

};

//-----------------------------------------------------------------------------------------------------------------------

int main (int argc, char *argv[]) {

	if (argc != 3) {
		
		fprintf(stderr, "Wrong number of function arguments");


		return ERR_ARG;
	}

	struct stat sbet;

	if (lstat(argv[1], &sbet) == -1) {
       
    perror("Error in lstat");

    return ERR_LST;
  }

	if ((sbet.st_mode & S_IFMT) != S_IFREG) {				
    		
    perror("Error in file type: non regular file");

		return ERR_FTY;
	}


	// r/w for user, read-only for group and others
	int fd_1 = open(argv[1], O_RDONLY);

	if (fd_1 < 0) {
			
		perror("Failed to open first file for copying");

		return ERR_FOF;
	}

	int fd_2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd_2 < 0) {
			
		perror("Failed to open second file for inserting");

		return ERR_FOS;
	}


	int num_of_copied_blocks = 0;

	ssize_t read_status = 1;

	int offset = 0;

	while (read_status > 0) {

		void *buf = calloc(4096, sizeof(char));

		read_status = pread(fd_1, buf, 4096, offset);

		if (read_status < 0) {

			perror("Error in file reading");
			free(buf);

			return ERR_FRE;
		}

		if (pwriteall(fd_2, buf, 4096, offset) < 0) {

			perror ("Error in file writing");
			free(buf);

			return ERR_FWR;
		} else { ++num_of_copied_blocks; }

		offset += 4096;
		free(buf);

	}

	if (read_status == 0) {
		puts("Copying completed");
	}

	printf("Number of blocks were processed: %d\n", num_of_copied_blocks);

	if (close(fd_1) < 0) {

		perror("Error in first file closing");

		return ERR_FFC;
	}

	if (close(fd_2) < 0) {

		perror("Error in second file closing");

		return ERR_SFC;
	}	



	return 0;
}