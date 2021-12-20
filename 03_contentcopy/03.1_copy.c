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

// function for convenient error handling
#define handle_error(msg) \
				do { int errno_code = errno; perror(msg); return errno_code; } while (0)

// function for convenient error handling with freeing ptr
#define handle_error_free(msg, ptr) \
				do { int errno_code = errno; perror(msg); free(ptr); return errno_code; } while (0)


//-----------------------------------------------------------------------------------------------------------------------
ssize_t writeall (int fd, const void *buf, size_t count) // function that controls the operation of write
{
	size_t bytes_written = 0;
	const uint8_t *buf_addr = buf;
	
	while (bytes_written < count) {
		
		ssize_t res = write(fd, buf_addr + bytes_written, count - bytes_written);
		
		if (res < 0) 
			return res;
		
		bytes_written += (size_t)res;
	}
	
	return (ssize_t)bytes_written;
}

//-----------------------------------------------------------------------------------------------------------------------
enum 
{

ERR_ARG_NUM = 1
};

//-----------------------------------------------------------------------------------------------------------------------

int main (int argc, char *argv[]) {

	if (argc != 3) { // checking the number of function arguments
		
		fprintf(stderr, "Wrong number of function arguments");


		return ERR_ARG_NUM;
	}

	struct stat sbet; // creating a structure to get information from lstat

	if (lstat(argv[1], &sbet) == -1) { // checking lstat completion status
       
    handle_error("Error in lstat");
  }

	if (S_ISREG(sbet.st_mode)) { // checking the file type 				
    		
    handle_error("Error in file type: non regular file");
	}


	// r/w for user, read-only for group and others
	int fd_1 = open(argv[1], O_RDONLY); // opening first file with reading only access mode

	if (fd_1 < 0) { // checking first file descriptor for errors
			
		handle_error("Failed to open first file for copying");
	}

	int fd_2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); // opening or creating second file for writing only
																																// O_TRUNC flag truncates existing file to zero length
	if (fd_2 < 0) { // checking second file descriptor for errors
			
		handle_error("Failed to open second file for inserting");
	}


	int num_of_copied_blocks = 0;

	ssize_t read_return = 1; // creating a variable to get read completion status
													 // default value is 1 to start while

	while (read_return > 0) {

		void *buf = calloc(BLOCK_SIZE, sizeof(char)); // allocating BLOCK_SIZE of memory for buffer

		read_return = read(fd_1, buf, BLOCK_SIZE); // getting read completion status

		if (read_return < 0) { // checking read completion status for errors
			
			handle_error_free("Error in file reading", buf);
		}

		if (writeall(fd_2, buf, (size_t)read_return) < 0) { // checking writeall completion status for errors

			handle_error_free("Error in file writing", buf);
		} else { ++num_of_copied_blocks; }

		free(buf);

	}

	if (read_return == 0) { // checking final completion status 
		puts("Copying completed");
	}

	printf("Number of blocks were processed: %d\n", num_of_copied_blocks);

	if (close(fd_1) < 0) { // checking closing first file descriptor for errors

		handle_error("Error in first file closing");

	}

	if (close(fd_2) < 0) { // checking closing second file descriptor for errors

		handle_error("Error in second file closing");

	}	

	return 0;
}