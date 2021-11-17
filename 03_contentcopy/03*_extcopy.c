
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#include <fcntl.h> //open
#include <unistd.h> //close

//-----------------------------------------------------------------------------------------------------------------------

enum error_codes
{

ERR_ARG = 1, // Error code 1  - wrong number of function arguments
ERR_LST,     // Error code 2  - failure in lstat work
ERR_FOF,     // Error code 3  - failed to open first file for copying
ERR_PRF,     // Error code 4  - failed to process regular file
ERR_FOS,     // Error code 5  - failed to open second file for inserting
ERR_FRE,     // Error code 6  - failure in file reading
ERR_FWR,     // Error code 7  - failure in file writing
ERR_FFC,     // Error code 8  - failed to close first file
ERR_SFC,     // Error code 9  - failed to close second file
ERR_PFI,		 // Error code 10 - failed to process fifo
ERR_PSL,     // Error code 11 - failed to process symlink
ERR_PBD,     // Error code 12 - failed to process block device
ERR_PCD,     // Error code 13 - failed to process character device
ERR_FCP,     // Error code 14 - failed to process file type
ERR_FCF,     // Error code 15 - failed to create FIFO file
ERR_RLE,     // Error code 16 - failed to read symlink with readlink
ERR_SLK,     // Error code 17 - failed to create symlink with symlink
ERR_MND,     // Error code 18 - failed to use mknode
ERR_PFT      // Error code 19 - failed to process file type





};


//-----------------------------------------------------------------------------------------------------------------------

ssize_t writeall (int fd, const void *buf, size_t count) 
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

int process_fifo (const char* newname, const struct stat* sb) {
	if (mkfifo(newname, sb->st_mode) < 0) {
		perror("Failed to create FIFO file\n");
		return ERR_FCF;
	}

	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------

int process_symlink (const char* pathname, const char* newname, const struct stat* sb) {
	size_t bufsiz;
	char* buf;

	bufsiz = (size_t)(sb->st_size + 1);
	buf = (char*)calloc(bufsiz, sizeof(char));
	if (buf == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	if (readlink(pathname, buf, bufsiz) < 0) {
		perror("readlink");
		free (buf);
		return ERR_RLE;
	}	

	if (symlink(buf, newname) < 0) {
		perror("symlink");
		free(buf);
		return ERR_SLK;
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int process_block_device (const char* newname, const struct stat* sb) {
	if (mknod(newname, sb->st_mode, sb->st_rdev) < 0) {
		perror("mknod");
		return ERR_MND;
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
int process_character_device (const char* newname, const struct stat* sb) {
	if (mknod(newname, sb->st_mode, sb->st_rdev) < 0) {
		perror("mknod");
		return ERR_MND;
	}

	return  0;
}

//-----------------------------------------------------------------------------------------------------------------------

int process_regular_file (const char* pathname, const char* newname) {


	// r/w for user, read-only for group and others
	int fd_1 = open(pathname, O_RDONLY);

	if (fd_1 < 0) {
			
		perror("Failed to open first file for copying");

		return ERR_FOF;
	}

	int fd_2 = open(newname, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd_2 < 0) {
			
		perror("Failed to open second file for inserting");

		return ERR_FOS;
	}


	int num_of_copied_blocks = 0;

	ssize_t read_status = 1;

	while (read_status > 0) {

		void *buf = calloc(4096, sizeof(char));

		read_status = read(fd_1, buf, 4096);

		if (read_status < 0) {

			perror("Error in file reading");
			free(buf);

			return ERR_FRE;
		}

		if (writeall(fd_2, buf, 4096) < 0) {

			perror ("Error in file writing");
			free(buf);

			return ERR_FWR;
		} else { ++num_of_copied_blocks; }

		free(buf);


	}

	if (read_status == 0) {
		puts("Copying of regular file completed");
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




//-----------------------------------------------------------------------------------------------------------------------

int processing_of_file_type (mode_t type, const char* argv_1, const char* argv_2, const struct stat* sb) {
	switch (type) {
		int return_code;
		case S_IFREG: 
			return_code = process_regular_file(argv_1, argv_2);
			
			if (return_code != 0) {
				fprintf(stderr, "Error in function process_regular_file. Error code:%d \n", return_code);
				return ERR_PRF;
			} 

			break;
		case S_IFIFO:
			return_code = process_fifo(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function process_fifo. Error code:%d \n", return_code);
				return ERR_PFI;
			} else {puts("Copy of FIFO completed");}

			break;
		case S_IFLNK:
			return_code = process_symlink(argv_1, argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function process_symlink. Error code:%d \n", return_code);
				return ERR_PSL;
			} else {puts("Copy of symlink completed");}

			break;
		case S_IFBLK:
			return_code = process_block_device(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function process_block_device. Error code:%d \n", return_code);
				return ERR_PBD;
			} else {puts("Copy of block device completed");}

			break;
		case S_IFCHR:
			return_code = process_character_device(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function process_character_device. Error code:%d \n", return_code);
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

	if (argc != 3) {
		
		fprintf(stderr, "Wrong number of function arguments");


		return ERR_ARG;
	}

	struct stat sbet;

	if (lstat(argv[1], &sbet) == -1) {
       
    perror("Error in lstat");

    return ERR_LST;
  }

	if (processing_of_file_type((sbet.st_mode & S_IFMT), argv[1], argv[2], &sbet) != 0) {
		
		perror("Error in processing_of_file_type");

		return  ERR_PFT;
	}

	return 0;
}