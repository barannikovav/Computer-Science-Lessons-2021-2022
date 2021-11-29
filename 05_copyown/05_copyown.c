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

// function for convenient error handling with freeing
#define handle_error_free(msg, ptr) \
				do { int errno_code = errno; perror(msg); free(ptr); return errno_code; } while (0)

//-----------------------------------------------------------------------------------------------------------------------

enum error_codes
{

	ERR_ARG = 1, // Error code 1  - wrong number of function arguments
	ERR_COF,		 // Error code 2 - failed to copy file
	ERR_CFP,     // Error code 3 - failed to copy file permissions
	ERR_CFO,		 // Error code 4 - failed to copy file owner

};


//-----------------------------------------------------------------------------------------------------------------------

ssize_t writeall (int fd, const void *buf, size_t count) // function that controls the operation of write
{
	size_t bytes_written = 0;
	const uint8_t *buf_addr = buf;
	
	while (bytes_written < count) {
		
		ssize_t res = write(fd, buf_addr + bytes_written, count - bytes_written);
		
		if (res < 0) {
			
			return res;

		}
		
		bytes_written += (size_t)res;
	}
	
	return (ssize_t)bytes_written;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_permissions (const int fd, const struct stat* sb) {
	if (fchmod(fd, sb->st_mode) < 0) { //using fchmod to change file permissions mode
		handle_error("Error in fchmod");
	}

	if (futimens(fd, (struct timespec[]){sb->st_atimespec, sb->st_mtimespec}) < 0) { // using futimens to set file access and modification time
		handle_error("Error in futimens");
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_own (const int fd, const struct stat* sb) {
	if (fchown(fd, sb->st_uid, sb->st_gid) < 0) {
		handle_error("Error in fchown");
	}

	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------

int copying_regular_file (const int fd_1_, const int fd_2_) {

	int num_of_copied_blocks = 0;

	ssize_t read_return = 1; // creating a variable to get read completion status
													 // default value is 1 to start while
	while (read_return > 0) {

		void *buf = calloc(BLOCK_SIZE, sizeof(char)); // allocating BLOCK_SIZE of memory for buffer

		read_return = read(fd_1_, buf, BLOCK_SIZE); // getting read completion status

		if (read_return < 0) { // checking read completion status for errors
			handle_error_free("Error in file reading", buf);
		}

		if (writeall(fd_2_, buf, BLOCK_SIZE) < 0) { // checking writeall completion status for errors
			handle_error_free("Error in file writing", buf);
		} else { ++num_of_copied_blocks; }

		free(buf);

	}

	if (read_return == 0) { // checking final completion status
		puts("Copying of regular file completed");
	}

	printf("Number of blocks were processed: %d\n", num_of_copied_blocks);

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------


int main (int argc, char *argv[]) {

	if (argc != 3) { // checking the number of function arguments
		
		fprintf(stderr, "Wrong number of function arguments");

		return ERR_ARG;
	}

	struct stat sbet; // creating a structure to get information from lstat

	if (lstat(argv[1], &sbet) == -1) { // checking lstat completion status
    handle_error("Error in lstat");  
  }

  if (!S_ISREG(sbet.st_mode)) { // checking the file type 				
    		
    handle_error("Error in file type: non regular file");
	}

		// r/w for user, read-only for group and others
	int fd_1 = open(argv[1], O_RDONLY);

	if (fd_1 < 0) { // checking first file descriptor for errors
		handle_error("Failed to open first file for copying");
	}

	int fd_2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); // opening or creating second file for writing only
																																// O_TRUNC flag truncates existing file to zero length
	if (fd_2 < 0) { // checking second file descriptor for errors
		handle_error("Failed to open second file for copying");	
	}

	if (copying_regular_file(fd_1, fd_2) != 0) {
		fprintf(stderr, "Error in copying_of_file");
		return ERR_COF;
	}

	if (copying_permissions(fd_2, &sbet) != 0) {
		fprintf(stderr, "Error in copying_permissions");
		return ERR_CFP;
	}

	if (copying_own(fd_2, &sbet) != 0) {
		fprintf(stderr, "Error in copying_own");
		return ERR_CFO;
	}

	if (close(fd_1) < 0) { // checking closing first file descriptor for errors
		handle_error("Error in first file closing");
	}

	if (close(fd_2) < 0) { // checking closing second file descriptor for errors

		handle_error("Error in second file closing");
	}

	return 0;
}