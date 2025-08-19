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

		int ret_val = 0;

		if (dprintf(fd, "%s",argv[2]) < 0) {//priting and checking the results
			
			perror("Failure while writing");
			ret_val = ERR_FWR;
		}

		if (close(fd) < 0) { //closing and checking the results
			perror("Failure while closing fd");

			ret_val = ERR_FIC;
		}

		return ret_val;
}