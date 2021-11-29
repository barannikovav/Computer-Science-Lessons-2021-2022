#include "./07_copydir_lib.h"

//-----------------------------------------------------------------------------------------------------------------------

enum failure_codes_main
{

	FAI_ARG = 1, // Failure code 1  - wrong number of function arguments
	FAI_CDC      // Failure code 2  - fail in copying directory content
};

//-----------------------------------------------------------------------------------------------------------------------

int main (int argc, char* argv[]) {
	const char * old_dir;
	const char * new_dir;

	if (argc != 3) { // checking the number of function arguments
		
		fprintf(stderr, "Wrong number of function arguments");

		return FAI_ARG;
	}

	old_dir = argv[1]; // getting dir names if number of arguments is ok 
	new_dir = argv[2];

	int old_dir_fd = open(old_dir, O_RDONLY); // getting fd pointer of dir program will copy form

	if (old_dir_fd == -1) // checking work of open for errors
		handle_error("open");

	struct stat dir_sb;

	if(fstat(old_dir_fd, &dir_sb) < 0) 
		handle_error("fstat");

	if (mkdir(new_dir, dir_sb.st_mode) < 0 && errno != EEXIST) { // if directory with name <old_dir> already exists, it will be 
		handle_error("mkdir");                               // opened. Otherwise, it will be created
	}

	int new_dir_fd = open(new_dir, O_RDWR); // getting fd pointer of dir program will copy to

	if (copying_dir_content(old_dir_fd, new_dir_fd) < 0) { // using function which copies directory content
		fprintf(stderr, "Failure in copying directory content\n");

		return FAI_CDC;
	}

  if (close(old_dir_fd) != 0 || close(new_dir_fd) != 0) /* closing descriptor with error checking */
  	handle_error("close");

	return 0;
}