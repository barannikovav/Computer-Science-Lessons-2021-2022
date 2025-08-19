#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#include <fcntl.h> //open
#include <unistd.h> //close

#define BLOCK_SIZE 4096

//-----------------------------------------------------------------------------------------------------------------------

enum 
{
ERR_ARG = 1, // Error code 1 - wrong number of function arguments
ERR_FILE_STAT,   // Error code 2 - failed to retrieve stat about the file
ERR_FILE_TYPE,   // Error code 3 - unsupported file type
ERR_FOFC,     	 // Error code 4 - failed to open file for copying
ERR_FOFI,     	 // Error code 5 - failed to open file for inserting
ERR_FRD,         // Error code 6 - failed to read file
ERR_FWR,     	 // Error code 7 - failed to write file
ERR_FIC,      	 // Error code 8 - failed to close file
};

//-----------------------------------------------------------------------------------------------------------------------

ssize_t pwriteall (int fd, const void *buf, size_t count, off_t offset) // function that controls the operation of pwrite
{
	size_t bytes_written = 0;
	const uint8_t *buf_addr = buf;
	
	while (bytes_written < count) {
		
		ssize_t res = pwrite(fd, buf_addr + bytes_written, count - bytes_written, offset);
		
		if (res < 0)
			return res;
		
		bytes_written += (size_t)res;
		offset += (int)res;
	}
	
	return (ssize_t)bytes_written;
}

//-----------------------------------------------------------------------------------------------------------------------

int main (int argc, char *argv[]) {

	if (argc != 3) { //checking the number of arguments
			
		fprintf(stderr, "Usage: %s SOURCE DEST\n", argv[0]);

		return ERR_ARG;
	}

	struct stat sbet; // creating a structure to get information from lstat

	if (lstat(argv[1], &sbet) == -1) { // checking lstat completion status
		
		perror("Failed to retrieve stat about the file");
		return ERR_FILE_STAT;
	}

	if (!S_ISREG(sbet.st_mode)) { // checking the file type 				
    	perror("Unsupported file type");
		return ERR_FILE_TYPE;
	}


	// r/w for user, read-only for group and others
	int fd_1 = open(argv[1], O_RDONLY); // opening first file with reading only access mode

	if (fd_1 < 0) { // checking first file descriptor for errors
			
		perror("Failed to open first file for copying");
		return ERR_FOFC;
	}

	int fd_2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); // opening or creating second file for writing only
																  // O_TRUNC flag truncates existing file to zero length
	if (fd_2 < 0) { // checking second file descriptor for errors

		perror("Failed to open second file for inserting");
		return ERR_FOFI;
	}


	int ret_val = EXIT_SUCCESS;

	int num_of_copied_blocks = 0;

	ssize_t read_return = 1;    // creating a variable to get read completion status
							    // default value is 1 to start while
	off_t offset = 0;

	void *buf = calloc(BLOCK_SIZE, sizeof(char)); // allocating BLOCK_SIZE of memory for buffer

	while (read_return > 0) {

		read_return = pread(fd_1, buf, BLOCK_SIZE, offset); // getting pread completion status

		if (read_return < 0) { // checking pread completion status for errors
			perror("Error in file reading");
			ret_val = ERR_FRD;
			break;
		}

		if (pwriteall(fd_2, buf, (size_t)read_return, offset) < 0) { // checking pwriteall completion status for errors
			perror("Error in file writing");
			ret_val = ERR_FWR;
			break;
		} else { ++num_of_copied_blocks; }

		offset += (off_t)read_return; // moving offset 
	}

	free(buf);

	if (read_return == 0) // checking final completion status 
		puts("Copying completed");
	else
		puts("Copying failed");

	printf("Number of blocks were processed: %d\n", num_of_copied_blocks);

	if (close(fd_1) < 0) { // checking closing first file descriptor for errors

		perror("Error in first file closing");
		ret_val = ERR_FIC;

	}

	if (close(fd_2) < 0) { // checking closing second file descriptor for errors

		perror("Error in second file closing");
		ret_val = ERR_FIC;

	}	

	return ret_val;
}