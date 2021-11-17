#include <string.h>
#include <stdint.h>

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //open
#include <unistd.h> //close
	

//-----------------------------------------------------------------------------------------------------------------------

enum 
{

ERR_ARG = 1, // Error code 1 - wrong number of function arguments
ERR_FOF,     // Error code 2 - failed to open file
ERR_FWR,     // Error code 3 - failed to write
ERR_FIC      // Error code 4 - failed to close file
     
};

//-----------------------------------------------------------------------------------------------------------------------

ssize_t writeall (int fd, const void *buf, size_t count) //function for reliable write
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

int main (int argc, char *argv[]) {

		if (argc != 3) { //checking the number of arguments
			
			fprintf(stderr, "Usage: %s filename text-to-write\n", argv[0]);

			return ERR_ARG;
		}

		//opening file and getting file descriptor || r/w for user, read-only for group and others
		int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd < 0) {//checking file descriptor
			
			perror("Failed to open file for writing");

			return ERR_FOF;
		}

		if (writeall(fd, argv[2], strlen(argv[2])) < 0) { //writing and checking the results
			
			perror("Failure while writing");
			close(fd);

			return ERR_FWR;
			}

		if (close(fd) < 0) { //closing and checking the results
			perror("Failure while closing fd");

			return ERR_FIC;
		}

		return 0;
}