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

#ifndef COPYLIB

#define COPYLIB

//-----------------------------------------------------------------------------------------------------------------------

enum error_codes
{

	ERR_ARG = 1,     // Error code 1  - wrong number of function arguments
    ERR_FILE_STAT,   // Error code 2  - failed to retrieve stat about the file
    ERR_FILE_TYPE,   // Error code 3  - unsupported file type
    ERR_FOFC,     	 // Error code 4  - failed to open file for copying
    ERR_FOFI,     	 // Error code 5  - failed to open file for inserting
    ERR_FRD,         // Error code 6  - failed to read file
    ERR_FWR,     	 // Error code 7  - failed to write file
    ERR_FIC,      	 // Error code 8  - failed to close file
    ERR_FCFI,        // Error code 9  - failed to create FIFO file
    ERR_SYMCAL,      // Error code 10 - calloc failure when allocating memory for new symlink
    ERR_SYMREAD,     // Error code 11 - readlink failure when reading source symlink
    ERR_SYMCRT,      // Error code 12 - symlink creation error
    ERR_MKBLOCK,     // Error code 13 - mknod failure when creating new block device
    ERR_MKCHA,       // Error code 14 - mknod failure when creating new character device
    ERR_PRF,         // Error code 15 - failed to process regular file
	ERR_PFI,	     // Error code 16 - failed to process fifo
	ERR_PSL,         // Error code 17 - failed to process symlink
	ERR_PBD,         // Error code 18 - failed to process block device
	ERR_PCD,         // Error code 19 - failed to process character device
	ERR_FCP,         // Error code 20 - failed to process file type
    ERR_CPY,         // Error code 21 - failed to copy file

};

//-----------------------------------------------------------------------------------------------------------------------
ssize_t writeall (int fd, const void *buf, size_t count);

int copy_regular_file (const char* pathname, const char* newname, void * buf, unsigned int buf_size);

int copy_fifo (const char* newname, const struct stat* sb);

int copy_symlink (const char* pathname, const char* newname, const struct stat* sb);

int copy_block_device (const char* newname, const struct stat* sb);

int copy_character_device (const char* newname, const struct stat* sb);

int copy_file (mode_t type, const char* argv_1, const char* argv_2, const struct stat* sb);

//-----------------------------------------------------------------------------------------------------------------------

#endif // COPYLIB