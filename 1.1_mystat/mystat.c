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

const char * File_type_switch (mode_t st_mode) { // switch function for type of file
        switch (st_mode & S_IFMT) {
           case S_IFBLK:  return "block device\n";            break;
           case S_IFCHR:  return "character device\n";        break;
           case S_IFDIR:  return "directory\n";               break;
           case S_IFIFO:  return "FIFO/pipe\n";               break;
           case S_IFLNK:  return "symlink\n";                 break;
           case S_IFREG:  return "regular file\n";            break;
           case S_IFSOCK: return "socket\n";                  break;
           default:       return "unknown?\n";                break;
        }

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

char * bad_ctime(struct timespec * time) { //function that is using instead of ctimes
        struct tm* t_time = localtime(&(time->tv_sec));
        long  int n_sec = time->tv_nsec;
        char* output = (char*)calloc(40, sizeof(char));

        strftime(output,  20, "%F %T", t_time);
        sprintf(output +  19, ".%ld", n_sec);
        strftime(output + 29, 7, " %z", t_time);

        return output;
}

//-----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
        {
        struct stat sb;

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

           printf("File type:                %s", File_type_switch(sb.st_mode));


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

           char * s = bad_ctime(&sb.st_atimespec);
           printf("Last status change:       %s\n", s);
           free(s);

           s = bad_ctime(&sb.st_ctimespec);
           printf("Last file access:         %s\n", s);
           free(s);

           s = bad_ctime(&sb.st_mtimespec);
           printf("Last file modification:   %s\n", s);
           free(s);

           s = bad_ctime(&sb.st_birthtimespec);
           printf("File creation(birth):     %s\n", s);
           free(s);






           exit(EXIT_SUCCESS);
       }