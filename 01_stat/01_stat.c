#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <string.h>
#include <stdlib.h>


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

char * access_convert (uintmax_t access_code) {//function that  convert access code to access string
        
        access_code = access_code & 0x1FF;//using mask here to decode 
        char * output;
        
        output = (char *) calloc(10, sizeof(char));//creating output string


        uintmax_t converter = 0x100;
        
        for (int i = 0; i < 9; ++i) { //inserting right sequence of chars in 'rwx' format
                if ((access_code & converter) == converter) {
                        if (i % 3 == 0) {
                                output[i] = 'r';
                        }
                        else if (i % 3 == 1) {
                                output[i] = 'w';
                        }
                        else {
                                output[i] = 'x';
                        }
                }
                else {
                       output[i] = '-'; 
                }
                converter = converter >> 1; 

        }

        return output;
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

           char * ptr = access_convert(sb.st_mode);

           printf("Access:                   (%jo/%s)\n", (uintmax_t) sb.st_mode & 0x1FF, ptr);

           free(ptr);

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