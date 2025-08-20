#include "copylib.h"

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

int copy_regular_file (const char* pathname, const char* newname, void * buf, unsigned int buf_size) {
    int ret_val = EXIT_SUCCESS;
    int num_of_copied_blocks = 0;

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

    ssize_t read_return = 1;    // creating a variable to get read completion status
							    // default value is 1 to start while

	void *buf = calloc(BLOCK_SIZE, sizeof(char)); // allocating BLOCK_SIZE of memory for buffer

	while (read_return > 0) {

		read_return = read(fd_1, buf, BLOCK_SIZE); // getting read completion status

		if (read_return < 0) { // checking read completion status for errors
			perror("Error in file reading");
			ret_val = ERR_FRD;
			break;
		}

		if (writeall(fd_2, buf, (size_t)read_return) < 0) { // checking writeall completion status for errors 
			perror("Error in file writing");
			ret_val = ERR_FWR;
			break;
		} else { ++num_of_copied_blocks; }

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

//-----------------------------------------------------------------------------------------------------------------------

int copy_fifo (const char* newname, const struct stat* sb) { // function for copying fifo with error checking
    int ret_val = EXIT_SUCCESS;
	if (mkfifo(newname, sb->st_mode) < 0) {
		perror("Failed to create FIFO file");
        ret_val = ERR_FCFI;
	}

	return ret_val;
}

//-----------------------------------------------------------------------------------------------------------------------

int copy_symlink (const char* pathname, const char* newname, const struct stat* sb) {
	int ret_val = EXIT_SUCCESS;
    size_t bufsiz;
	char* buf;

	bufsiz = (size_t)(sb->st_size + 1); // setting size of a buffer for symlink
	buf = (char*)calloc(bufsiz, sizeof(char)); // creating a buffer for symlink
	
    if (buf == NULL) { // checking buf ptr for errors
		perror("Calloc failure when allocating memory for new symlink");
        ret_val = ERR_SYMCAL;
        goto CLEANUP;
	}

	if (readlink(pathname, buf, bufsiz) < 0) { // checking readlink for errors
		perror("Readlink failure when reading source symlink");
        ret_val = ERR_SYMREAD;
        goto CLEANUP;
	}	

	if (symlink(buf, newname) < 0) { // checking symlink for errors
		perror("Symlink creation error");
        ret_val = ERR_SYMCRT;
	}

    CLEANUP:
	free(buf);
	return ret_val;
}

//-----------------------------------------------------------------------------------------------------------------------

int copy_block_device (const char* newname, const struct stat* sb) { // function for copying block device with error checking
	int ret_val = EXIT_SUCCESS;
    if (mknod(newname, sb->st_mode, sb->st_rdev) < 0) {
		perror("Mknod failure when creating new block device");
        ret_val = ERR_MKBLOCK;
	}

	return ret_val;
}

//-----------------------------------------------------------------------------------------------------------------------

int copy_character_device (const char* newname, const struct stat* sb) { // function for character device with error checking
	int ret_val = EXIT_SUCCESS;
    if (mknod(newname, sb->st_mode, sb->st_rdev) < 0) {
		perror("Mknod failure when creating new character device");
        ret_val = ERR_MKCHA;
	}

	return ret_val;
}

//-----------------------------------------------------------------------------------------------------------------------

int copy_file (mode_t type, const char* argv_1, const char* argv_2, const struct stat* sb) {
	void * buffer = NULL;

	switch (type) {
		int return_code; // variable that  uses to contain return codes of copying functions
		
		case S_IFREG: 
			return_code = copy_regular_file(argv_1, argv_2, buffer, BLOCK_SIZE);
			
			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_regular_file. Error code:%d \n", return_code);
				return ERR_PRF;
			} 

			break;
		case S_IFIFO:
			return_code = copy_fifo(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_fifo. Error code:%d \n", return_code);
				return ERR_PFI;
			} else {puts("Copy of FIFO completed");}

			break;
		case S_IFLNK:
			return_code = copy_symlink(argv_1, argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_symlink. Error code:%d \n", return_code);
				return ERR_PSL;
			} else {puts("Copy of symlink completed");}

			break;
		case S_IFBLK:
			return_code = copy_block_device(argv_2, sb);

			if (return_code != 0) {
				fprintf(stderr, "Error in function copying_block_device. Error code:%d \n", return_code);
				return ERR_PBD;
			} else {puts("Copy of block device completed");}

			break;
		case S_IFCHR:
			return_code = copy_character_device(argv_2, sb);

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