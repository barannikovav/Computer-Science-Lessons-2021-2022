#include "./07_copydir_lib.h"

//-----------------------------------------------------------------------------------------------------------------------

enum failure_codes_main
{

	FAI_ARG = 1, // Failure code 1  - wrong number of function arguments
	FAI_COPYING_CONTENT,      // Failure code 2  - fail in copying directory content
	FAI_CREATING_DIRECTORY,
	FAI_OPENING_DIRECTORY,
	FAI_CLOSING_DIRECTORY,
};

//-----------------------------------------------------------------------------------------------------------------------

int main (int argc, char* argv[]) {
	const char * old_dir;
	const char * new_dir;

	int exit_code = 0;

	if (argc != 3) { // checking the number of function arguments
		
		fprintf(stderr, "Wrong number of function arguments");

		return FAI_ARG;
	}

	old_dir = argv[1]; // getting dir names if number of arguments is ok 
	new_dir = argv[2];

	DIR* old_dir_ptr = opendir(old_dir); // getting DIR pointer of dir program will copy form

	if (old_dir_ptr == NULL) // checking work of open for errors
		handle_error("opendir");

	struct stat dir_sb;

	if(fstat(dirfd(old_dir_ptr), &dir_sb) < 0)  {

		handle_error("fstat");

	} else {

		if (mkdir(new_dir, dir_sb.st_mode) < 0 && errno != EEXIST) { // if directory with name <old_dir> already exists, it will be
			                                                           // opened. Otherwise, it will be created
			perror("mkdir"); 

			exit_code = FAI_CREATING_DIRECTORY;

		} else {

			DIR* new_dir_ptr = opendir(new_dir); // getting dir pointer of dir program will copy to

			if (new_dir_ptr == NULL) { // checking work of open for errors

				perror("opendir");

				exit_code = FAI_OPENING_DIRECTORY;

			} else {

				if (copying_dir_content_recursively(old_dir_ptr, new_dir_ptr) < 0) { // using function which copies directory content
				
					fprintf(stderr, "Failure in copying directory content\n");

					exit_code = FAI_COPYING_CONTENT;

				}

			}

		}

	}

  if (closedir(old_dir_ptr) != 0) { /* closing descriptor with error checking */
  	perror("closedir");
  	exit_code = FAI_CLOSING_DIRECTORY;
  }

  if (closedir(new_dir_ptr) != 0)
  	perror("closedir");
  	exit_code = FAI_CLOSING_DIRECTORY;

	return exit_code;
}