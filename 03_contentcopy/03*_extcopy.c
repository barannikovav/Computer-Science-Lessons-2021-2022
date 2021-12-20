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
	ERR_PRF,     // Error code 2  - failed to process regular file
	ERR_PFI,		 // Error code 3 - failed to process fifo
	ERR_PSL,     // Error code 4 - failed to process symlink
	ERR_PBD,     // Error code 5 - failed to process block device
	ERR_PCD,     // Error code 6 - failed to process character device
	ERR_FCP,     // Error code 7 - failed to process file type

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

int copying_fifo (const char* newname, const struct stat* sb) { // function for copying fifo with error checking
	if (mkfifo(newname, sb->st_mode) < 0) {
		handle_error("Failed to create FIFO file");
	}

	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------

int copying_symlink (const char* pathname, const char* newname, const struct stat* sb) {
	size_t bufsiz;
	char* buf;

	bufsiz = (size_t)(sb->st_size + 1); // setting size of a buffer for symlink
	buf = (char*)calloc(bufsiz, sizeof(char)); // creating a buffer for symlink
	if (buf == NULL) { // checking buf ptr for errors
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	if (readlink(pathname, buf, bufsiz) < 0) { // checking readlink for errors
		perror("readlink");
	}	

	if (symlink(buf, newname) < 0) { // checking symlink for errors
		perror("readlink");
	}

	free(buf);

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_block_device (const char* newname, const struct stat* sb) { // function for copying block device with error checking
	if (mknod(newname, sb->st_mode, sb->st_rdev) < 0) {
		handle_error("mknod");
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
int copying_character_device (const char* newname, const struct stat* sb) { // function for character device with error checking
	if (mknod(newname, sb->st_mode, sb->st_rdev) < 0) {
		handle_error("mknod");
	}

	return  0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_regular_file (const char* pathname, const char* newname, void * buf, unsigned int buf_size) {
  int exit_code;
  int num_of_copied_blocks = 0;

  // r/w for user, read-only for group and others
	int fd_1 = open(pathname, O_RDONLY);
	int fd_2 = 0;

  if (fd_1 < 0){
    exit_code = errno;
    perror("Failed to open first file for copying");
  } else {
    fd_2 = open(newname, O_WRONLY | O_CREAT | O_TRUNC, 0644); // opening or creating second file for writing only

    if (fd_2 < 0) { // checking second file descriptor for errors
    	exit_code = errno; 
    	perror("Failed to open second file for copying");   
  	} else {
      	ssize_t read_return = 1; // creating a variable to get read completion status
        	                       // default value is 1 to start while

      	buf = malloc(buf_size * sizeof(char)); // allocating BLOCK_SIZE of memory for buffer

      	while (read_return > 0) {

	        read_return = read(fd_1, buf, buf_size); // getting read completion status

  	      if (read_return < 0) { // checking read completion status for errors
    	      handle_error("Error in file reading");
      	  }

        	if (writeall(fd_2, buf, buf_size) < 0) { // checking writeall completion status for errors
          	handle_error("Error in file writing");
        	} else { ++num_of_copied_blocks; }

      	}

      	free(buf);

      	if (read_return == 0) { // checking final completion status
					puts("Copying of regular file completed");
				}

				printf("Number of blocks were processed: %d\n", num_of_copied_blocks);
     	}
    }

    

    if (close(fd_1) < 0) { // checking closing first file descriptor for errors
        perror("Error in first file closing");
    }

    if (close(fd_2) < 0) { // checking closing second file descriptor for errors

        perror("Error in second file closing");
    }


    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------
/*
int copying_regular_file (const char* pathname, const char* newname, void * buf, unsigned int buf_size) {

	int num_of_copied_blocks = 0;

	// r/w for user, read-only for group and others
	int fd_1 = open(pathname, O_RDONLY);

	if (fd_1 < 0) { // checking first file descriptor for errors
		handle_error("Failed to open first file for copying");
	}

	int fd_2 = open(newname, O_WRONLY | O_CREAT | O_TRUNC, 0644); // opening or creating second file for writing only
																																// O_TRUNC flag truncates existing file to zero length
	if (fd_2 < 0) { // checking second file descriptor for errors
		handle_error("Failed to open second file for copying");	
	}


	int num_of_copied_blocks = 0;

	ssize_t read_return = 1; // creating a variable to get read completion status
													 // default value is 1 to start while
	while (read_return > 0) {

		buf = calloc(buf_size, sizeof(char)); // allocating buf_size of memory for buffer

		read_return = read(fd_1, buf, buf_size); // getting read completion status

		if (read_return < 0) { // checking read completion status for errors
			handle_error_free("Error in file reading", buf);
		}

		if (writeall(fd_2, buf, buf_size) < 0) { // checking writeall completion status for errors
			handle_error_free("Error in file writing", buf);
		} else { ++num_of_copied_blocks; }

		free(buf);


	}

	if (read_return == 0) { // checking final completion status
		puts("Copying of regular file completed");
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
*/
//-----------------------------------------------------------------------------------------------------------------------

int copying_of_file (mode_t type, const char* argv_1, const char* argv_2, const struct stat* sb) {
	void * buffer = NULL;

	switch (type) {
		int return_code; // variable that  uses to contain return codes of copying functions
		
		case S_IFREG: 
			return_code = copying_regular_file(argv_1, argv_2, buffer, BLOCK_SIZE);
			
			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_regular_file. Error code:%d \n", return_code);
				return ERR_PRF;
			} 

			break;
		case S_IFIFO:
			return_code = copying_fifo(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_fifo. Error code:%d \n", return_code);
				return ERR_PFI;
			} else {puts("Copy of FIFO completed");}

			break;
		case S_IFLNK:
			return_code = copying_symlink(argv_1, argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_symlink. Error code:%d \n", return_code);
				return ERR_PSL;
			} else {puts("Copy of symlink completed");}

			break;
		case S_IFBLK:
			return_code = copying_block_device(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_block_device. Error code:%d \n", return_code);
				return ERR_PBD;
			} else {puts("Copy of block device completed");}

			break;
		case S_IFCHR:
			return_code = copying_character_device(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_character_device. Error code:%d \n", return_code);
				return ERR_PCD;
			} else {puts("Copy of character device completed");}

			break;
		default:
			perror("Error. The file type cannot be processed by the program.");
			return ERR_FCP;

	}

	return 0;
}


int main (int argc, char *argv[]) {

	if (argc != 3) { // checking the number of function arguments
		printf("Usage: %s <source> <destination>\n", argv[0]);
		return ERR_ARG;
	}

	struct stat sbet; // creating a structure to get information from lstat

	if (lstat(argv[1], &sbet) == -1) { // checking lstat completion status
    handle_error("Error in lstat");  
  }

	if (copying_of_file((sbet.st_mode & S_IFMT), argv[1], argv[2], &sbet) != 0) {
		handle_error("Error in processing_of_file_type");
	}

	return 0;
}