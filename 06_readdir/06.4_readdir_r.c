#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h> // to return errno
#include <fcntl.h>
#include <string.h>

// function for convenient error handling
#define handle_error(msg) \
				do { int errno_code = errno; perror(msg); return errno_code; } while (0)

//-----------------------------------------------------------------------------------------------------------------------

/* function for determining the file type 
   by the field d_type of dirent structure; 
   not supported by all file systems       */

char dtype_char(unsigned dtype) { 
	switch (dtype) {
		case DT_BLK:   	 return 'b'; /* block special */
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

int scan_dir (const int fd, unsigned layer) {

   DIR *dir_fd = fdopendir(fd); /* getting dir descriptor by it's path */

   if (dir_fd == NULL) /* checking opendir output for errors */
      handle_error("fdopendir");

   struct dirent* entry;

   while ((entry = readdir(dir_fd)) != NULL) { /* scanning dir until end or error */
      errno = 0;

      if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
         
         char entry_type = dtype_char(entry->d_type); /* getting file type with dtype */
         if (entry_type == '?') { /* in case of unknown type/error one more check with stat */

            struct stat sb;

            if (fstatat(fd, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW) < 0)
               perror("fstatat");
            else 
               entry_type = mode_char(sb.st_mode);

         }
         
         printf("%c|  ", entry_type);

         if(layer > 0) {
            for (unsigned i = 0; i < layer; ++i)
               printf("|  ");
         }

         printf("%s\n", entry->d_name);

         if (entry_type == 'd') {
            int deeper_dirfd = openat(fd, entry->d_name, O_RDONLY);
            scan_dir(deeper_dirfd, layer + 1);
         }



      } else {
         continue;
      }

   }

   if (closedir(dir_fd) != 0) /* closing descriptor with error checking */
      handle_error("closedir_2");

   return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int main (int argc, char* argv[]) {
   int exit_code = 0;

   int fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY); /* getting fd descriptor */
  
   if (fd == -1)                                                     /* O_RDONLY - read-only perms */
      handle_error("open");                                      

   unsigned layer = 0;

   exit_code = scan_dir(fd, layer);

   return exit_code;
}