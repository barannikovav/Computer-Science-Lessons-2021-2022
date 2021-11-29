#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h> // to return errno
#include <fcntl.h>

// function for convenient error handling
#define handle_error(msg) \
				do { int errno_code = errno; perror(msg); return errno_code; } while (0)

//-----------------------------------------------------------------------------------------------------------------------

/* function for determining the file type 
   by the field d_type of dirent structure; 
   not supported by all file systems       */

char dtype_char(unsigned dtype) { 
	switch (dtype) {
		case DT_BLK:     return 'b'; /* block special */
		case DT_CHR:     return 'c'; /* character special */
		case DT_DIR:     return 'd'; /* directory */
		case DT_FIFO:    return 'p'; /* named pipe (fifo) */
		case DT_LNK:     return 'l'; /* symbolic link */
		case DT_REG:     return '-'; /* regular */
		case DT_SOCK:    return 's'; /* socket */
	}

	return '?';
}

//-----------------------------------------------------------------------------------------------------------------------

/* function for determining the file type 
   by the field st_mode of stat structure; 
   created because determining by d_type 
   not supported by all file systems      */

char mode_char(unsigned mode) {
    switch (mode & S_IFMT)
    {
        case S_IFBLK:   return 'b'; /* block special */
        case S_IFCHR:   return 'c'; /* character special */
        case S_IFDIR:   return 'd'; /* directory */
        case S_IFIFO:   return 'p'; /* named pipe (fifo) */
        case S_IFLNK:   return 'l'; /* symbolic link */
        case S_IFREG:   return '-'; /* regular */
        case S_IFSOCK:  return 's'; /* socket */
    }
    return '?';
}

//-----------------------------------------------------------------------------------------------------------------------

int main (void) {
	int errno_code = 0;

	DIR *dir_fd = opendir("."); /* getting dir descriptor by it's path */

	if (dir_fd == NULL) /* checking opendir output for errors */
		handle_error("opendir");

	

	struct dirent *entry;

	while ((entry = readdir(dir_fd)) != NULL) { /* scanning dir until end or error */

		char entry_type = dtype_char(entry->d_type); /* getting file type with dtype */
		if (entry_type == '?') { /* in case of unknown type/error one more check with stat */
			
			struct stat sb;
			if (lstat(entry->d_name, &sb) < 0) 
				handle_error("fstatat");
			else 
				entry_type = mode_char(sb.st_mode);

			
		}
		printf("%c %s \n", entry_type, entry->d_name);

	}

	errno_code = errno;
	if (errno_code != 0) { /* checking readdir for errors */

		perror("readdir");

		return errno_code;
	}

	if (closedir(dir_fd) != 0) /* closing descriptor with error checking */
		handle_error("closedir");


	return 0;
}