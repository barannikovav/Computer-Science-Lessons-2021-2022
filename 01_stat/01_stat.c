#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <string.h>
#include <stdlib.h>

#define PERMS 0x1FF


//-----------------------------------------------------------------------------------------------------------------------

const char * File_type_switch (unsigned mode) { // switch function for type of file
        switch (mode & S_IFMT) {
           case S_IFBLK:  return "block device";            
           case S_IFCHR:  return "character device";        
           case S_IFDIR:  return "directory";               
           case S_IFIFO:  return "FIFO/pipe";               
           case S_IFLNK:  return "symlink";                 
           case S_IFREG:  return "regular file";            
           case S_IFSOCK: return "socket";                                       
        }

        return "unknown?";

}

//-----------------------------------------------------------------------------------------------------------------------

void perms(char* buf, const unsigned mode)
{
    buf[0] = mode & S_IRUSR ? 'r' : '-';
    buf[1] = mode & S_IWUSR ? 'w' : '-';
    buf[2] = mode & S_IXUSR ? 'x' : '-';
    buf[3] = mode & S_IRGRP ? 'r' : '-';
    buf[4] = mode & S_IWGRP ? 'w' : '-';
    buf[5] = mode & S_IXGRP ? 'x' : '-';
    buf[6] = mode & S_IROTH ? 'r' : '-';
    buf[7] = mode & S_IWOTH ? 'w' : '-';
    buf[8] = mode & S_IXOTH ? 'x' : '-';
    buf[9] = '\0';
}

//-----------------------------------------------------------------------------------------------------------------------

int my_ctime(char* out_buf, size_t buf_size, const struct timespec * file_timestamp) { //function that is using instead of ctimes
        
        struct tm* t_time = localtime(&(file_timestamp->tv_sec));
        
        char yyyymmdd_hhmmss[sizeof("YYYY-mm-dd HH:MM:SS")];
        char tz_str[sizeof("+hhmm")];

        strftime(yyyymmdd_hhmmss, sizeof(yyyymmdd_hhmmss), "%F %T", t_time);

        return snprintf(out_buf, buf_size, "%s.%09ld %s", yyyymmdd_hhmmss, file_timestamp->tv_nsec, tz_str);
}

//-----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
        {
        struct stat sb;
        char string[sizeof("YYYY-mm-dd HH:MM:SS.nnnnnnnnn +hhmm")];
        char perm_str[sizeof("rwxrwxrwx")];


           if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (lstat(argv[1], &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
           }

           printf("Name of file:             %s\n", argv[1]);

           printf("ID of containing device:  [%jx,%jx]\n",
                   (uintmax_t) major(sb.st_dev),
                   (uintmax_t) minor(sb.st_dev));

           printf("File type:                %s\n]", File_type_switch(sb.st_mode));


           printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);

           perms(perm_str, sb.st_mode);

           printf("Access:                   (%jo/%s)\n", (uintmax_t) sb.st_mode & PERMS, perm_str);

           printf("Mode:                     %jo (octal)\n",
                   (uintmax_t) sb.st_mode);

           printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);
           printf("Ownership:                UID=%ju   GID=%ju\n",
                   (uintmax_t) sb.st_uid, (uintmax_t) sb.st_gid);

           printf("Preferred I/O block size: %jd bytes\n",
                   (intmax_t) sb.st_blksize);
           printf("File size:                %jd bytes\n",
                   (intmax_t) sb.st_size);
           printf("Blocks allocated:         %jd\n",
                   (intmax_t) sb.st_blocks);
           
           tzset();

           my_ctime(string, sizeof(string), &sb.st_ctimespec);
           printf("Last status change:       %s\n", string);

           my_ctime(string, sizeof(string), &sb.st_atimespec);
           printf("Last file access:         %s\n", string);

           my_ctime(string, sizeof(string), &sb.st_mtimespec);
           printf("Last file modification:   %s\n", string);

           my_ctime(string, sizeof(string), &sb.st_birthtimespec);
           printf("File creation(birth):     %s\n", string);

           exit(EXIT_SUCCESS);
       }