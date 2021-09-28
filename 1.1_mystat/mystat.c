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

const char * File_type_switch (mode_t st_mode) { 
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

char * access_convert (uintmax_t access_code) {
        access_code = access_code & 0x1FF;
        char * output;
        
        output = (char *) calloc(10, sizeof(char));


        uintmax_t converter = 0x100;
        
        for (int i = 0; i < 9; ++i) {
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

char * bad_ctime(struct timespec * time) {
        char * output;
        char first_part[20];
        output = (char *) calloc(32, sizeof(char));
        for (int i = 0; i < 19; ++i) {
                first_part[i] = *(ctime(&time->tv_sec) + i);
        }
        first_part[19] = '.';
        char second_part[4]; 
        for (int j = 20; j < 24; ++j) {
                second_part[j - 20] = *(ctime(&time->tv_sec) + j);
        }
        
        sprintf(output, "%s", first_part);
        int time_zone = (int)-timezone/3600;
        sprintf(&output[20], "%li %s%d %s %s", time->tv_nsec, "UTM +", time_zone, second_part, "\n");

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
           printf("Last status change:       %s", s);
           free(s);

           s = bad_ctime(&sb.st_ctimespec);
           printf("Last file access:         %s", s);
           free(s);

           s = bad_ctime(&sb.st_mtimespec);
           printf("Last file modification:   %s", s);
           free(s);

           s = bad_ctime(&sb.st_birthtimespec);
           printf("File creation(birth):     %s", s);
           free(s);






           exit(EXIT_SUCCESS);
       }