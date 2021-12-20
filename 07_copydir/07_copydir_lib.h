#define _ATFILE_SOURCE

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h> // for dirent struct

#include <unistd.h> //close
#include <fcntl.h>  //open

#include <errno.h> // to return errno

#define BLOCK_SIZE 4096

// function for convenient error handling
#define handle_error(msg) \
                do { int errno_code = errno; perror(msg); return errno_code; } while (0)

// function for convenient error handling with freeing
#define handle_error_free(msg, ptr) \
                do { int errno_code = errno; perror(msg); free(ptr); return errno_code; } while (0)

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

enum error_codes_copying
{

    ERR_ARG = 1, // Error code 1  - wrong number of function arguments
    ERR_CFC,         // Error code 2 - failed to copy file content
    ERR_CFP,     // Error code 3 - failed to copy file permissions
    ERR_CFO,         // Error code 4 - failed to copy file owner

};

//-----------------------------------------------------------------------------------------------------------------------

int copying_permissions_at (int dirfd, const char * filepath, const struct stat* sb) {
    int fd = openat(dirfd, filepath, O_RDONLY);

    if (fd < 0) { // checking file descriptor for errors

        perror("openat");

    } else {

        if (fchmodat(dirfd, filepath, sb->st_mode % 1000 > 292 ? sb->st_mode : 420, AT_SYMLINK_NOFOLLOW) < 0) { // using fchmodat to change file permissions mode
                                                                                 // if permissions are worse then 444, we set them by default 644
            perror("fchmod");

        } else {

            if (utimensat(dirfd, filepath, (struct timespec[]){sb->st_atim, sb->st_mtim}) < 0, AT_SYMLINK_NOFOLLOW) { // using futimens to set file access and modification time

                perror("futimens");

            }
        }

    }

    close(fd);

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_own_at (int dirfd, const char * filepath, const struct stat* sb) {

    if (fchownat(dirfd, filepath, sb->st_uid, sb->st_gid, 0) < 0) {
        handle_error("Error in fchownat");
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

ssize_t writeall (int fd, const void *buf, size_t count) // function that controls the operation of write
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

int copying_fifo_at (int dirfd, const char* newname, const struct stat* sb) { // function for copying fifo with error checking
    if (mkfifoat(dirfd, newname, sb->st_mode) < 0) {
        perror("Failed to create FIFO file");
    }

    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------

int copying_symlink_at (int dirfd_start, int dirfd_destination, const char* pathname, const char* newname, const struct stat* sb) {
    size_t bufsiz;
    char* buf;

    bufsiz = (size_t)(sb->st_size > 0 ? sb->st_size + 1 : BLOCK_SIZE); // setting size of a buffer for symlink
    buf = (char*)calloc(bufsiz, sizeof(char)); // creating a buffer for symlink
    if (buf == NULL) { // checking buf ptr for errors
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    if (readlinkat(dirfd_start, pathname, buf, bufsiz) < 0) { // reading symlink and checking for errors
        handle_error_free("readlink", buf);
    }   

    if (symlinkat(buf, dirfd_destination, newname) < 0) { // creating symlink and checking for errors
        handle_error_free("symlink", buf);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_block_and_character_device_at (int dirfd, const char* newname, const struct stat* sb) { // function for copying block and character device with error checking
    if (mknodat(dirfd, newname, sb->st_mode, sb->st_rdev) < 0) {
        handle_error("mknodat");
    }

    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------

int copying_regular_file_at (int dirfd_start, int dirfd_destination, const char* pathname, const char* newname) {


    // r/w for user, read-only for group and others
    int fd_1 = openat(dirfd_start, pathname, O_RDONLY);

    if (fd_1 < 0) { // checking first file descriptor for errors
        handle_error("Failed to open first file for copying");
    }

    int fd_2 = openat(dirfd_destination, newname, O_WRONLY | O_CREAT | O_TRUNC, 0644); // opening or creating second file for writing only
                                                                                                                                                                     // O_TRUNC flag truncates existing file to zero length
    if (fd_2 < 0) { // checking second file descriptor for errors
        handle_error("Failed to open second file for copying"); 
    }


    ssize_t read_return = 1; // creating a variable to get read completion status
                                                     // default value is 1 to start while
    while (read_return > 0) {

        void *buf = calloc(BLOCK_SIZE, sizeof(char)); // allocating BLOCK_SIZE of memory for buffer

        read_return = read(fd_1, buf, BLOCK_SIZE); // getting read completion status

        if (read_return < 0) { // checking read completion status for errors
            handle_error_free("Error in file reading", buf);
        }

        if (writeall(fd_2, buf, BLOCK_SIZE) < 0) { // checking writeall completion status for errors
            handle_error_free("Error in file writing", buf);
        }

        free(buf);


    }

    if (close(fd_1) < 0) { // checking closing first file descriptor for errors
        handle_error("Error in first file closing");
    }

    if (close(fd_2) < 0) { // checking closing second file descriptor for errors

        handle_error("Error in second file closing");
    }


    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_dir_at (int dirfd, const char * newname, const struct stat* sb) {
    if ()

    if (mkdirat(dirfd, newname, sb->st_mode % 1000 > 292 ? sb->st_mode : 420) < 0 && errno != EEXIST) {
        handle_error("mkdirat");
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_of_file (int start_fd, int dest_fd, mode_t type, const char* old_name, const char* new_name, const struct stat* sb) {
    int return_code; // variable that uses to contain return codes of copying functions

    switch (type & S_IFMT) {
        
        case S_IFREG: 
            return_code = copying_regular_file_at(start_fd, dest_fd, old_name, new_name);
            
            if (return_code != 0) {
                fprintf(stderr, "Error in function copying_regular_file_at. Error code:%d \n", return_code);
                return ERR_CFC;
            } 

            break;
        case S_IFIFO:
            return_code = copying_fifo_at(dest_fd, new_name, sb);

            if (return_code != 0) {
                fprintf(stderr, "Error in function copying_fifo_at. Error code:%d \n", return_code);
                return ERR_CFC;
            }

            break;
        case S_IFLNK:
            return_code = copying_symlink_at(start_fd, dest_fd, old_name, new_name, sb);

            if (return_code != 0) {
                fprintf(stderr, "Error in function copying_symlink_at. Error code:%d \n", return_code);
                return ERR_CFC;
            }

            break;
        case S_IFBLK:
        case S_IFCHR:
            return_code = copying_block_and_character_device_at(dest_fd, new_name, sb);

            if (return_code != 0) {
                fprintf(stderr, "Error in function copying_block_and_character_device_at. Error code:%d \n", return_code);
                return ERR_CFC;
            }

            break;
        case S_IFDIR:
            return_code = copying_dir_at(dest_fd, new_name, sb);

            if (return_code != 0) {
                fprintf(stderr, "Error in function copying_dir_at. Error code:%d \n", return_code);
                return ERR_CFC;
            }

            break;
        default:
            perror("Error. The file type cannot be processed by the program.");
            return ERR_CFC;

    }

    return_code = copying_permissions_at(dest_fd, new_name, sb);

    if (return_code != 0) {
        fprintf(stderr, "Error in function copying_permissions_at. Error code:%d \n", return_code);
        return ERR_CFP;
    }

    return_code = copying_own_at(dest_fd, new_name, sb);

    if (return_code != 0) {
        fprintf(stderr, "Error in function copying_own_at. Error code:%d \n", return_code);
        return ERR_CFO;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

int copying_dir_content (DIR* dir_start, DIR* dir_destination) {

    const int dirfd_start = dirfd(dir_start); /* getting dir descriptor by it's path */
    if (dirfd_start < 0) /* checking opendir output for errors */
        handle_error("dirfd dirfd_start");

    const int dirfd_destination = dirfd(dir_destination);
    if (dirfd_destination < 0) 
        handle_error("dirfd dirfd_destination");

  struct dirent* entry;

  while ((entry = readdir(dir_start)) != NULL) { /* scanning dir until end or error */
    struct stat sb;

    if (strcmp(entry->d_name, ".") || strcmp(entry->d_name, "..")) {
        continue
    }

    if (fstatat(dirfd_start, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW) < 0) 
        handle_error("fstatat");

    if (copying_of_file(dirfd_start, dirfd_destination, sb.st_mode, entry->d_name, entry->d_name, &sb) != 0) {
        fprintf(stderr, "Failed to copy %s\n", entry->d_name);
    }

    }

  return 0;
}

//-----------------------------------------------------------------------------------------------------------------------


int copying_dir_content_recursively (DIR* dir_start, DIR* dir_destination) {
    exit_code = 0;

    const int dirfd_start = dirfd(dir_start); /* getting dir descriptor by it's path */
    if (dir_start == NULL)
        handle_error("fdopendir dir_start");

    const int dirfd_destination = dirfd(dir_destination);

    if (dir_destination == NULL) /* checking opendir output for errors */
        handle_error("fdopendir dir_destination");

    struct dirent* entry;

    while ((entry = readdir(dir_start)) != NULL) { /* scanning dir until end or error */
        struct stat sb;

        if (strcmp(entry->d_name, ".") || strcmp(entry->d_name, "..")) {
            continue;
        } else { 

             if (fstatat(dirfd_start, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW) < 0) {

                exit_code = errno;
                perror("fstatat");

             } else {

                if (copying_of_file(dirfd_start, dirfd_destination, sb.st_mode, entry->d_name, entry->d_name, &sb) != 0) {

                    fprintf(stderr, "Failed to copy %s\n", entry->d_name);

                }

                if (S_ISDIR(sb.st_mode)) {

                    const int deeper_old_dir_ptr_fd = openat(dirfd_start, entry->d_name, O_RDONLY | O_DIRECTORY); // getting fd pointer of deeper dir program will copy from

                    if (deeper_old_dir_ptr_fd < 0) { // checking file descriptor for errors

                        exit_code = 0;
                        perror("openat deeper_old_dir_ptr_fd");

                    } else {

                        DIR * deeper_old_dir_ptr = fdopendir(deeper_old_dir_ptr_fd);

                        if (deeper_old_dir_ptr == NULL) { // checking work of fdopen for errors
                            
                            exit_code = errno;
                            perror("fdopendir deeper_old_dir_ptr")

                        } else {

                            const int deeper_new_dir_ptr_fd = openat(dirfd_destination, entry->d_name, O_RDONLY | O_DIRECTORY);

                            if (deeper_old_dir_ptr_fd < 0) { // checking file descriptor for errors

                                exit_code = errno;
                                perror("openat deeper_old_dir_ptr_fd");

                            } else {

                                DIR * deeper_old_dir_ptr = fdopendir(deeper_old_dir_ptr_fd);

                                if (deeper_old_dir_ptr == NULL) { // checking work of open for errors

                                    exit_code = errno;
                                    perror("fdopendir deeper_old_dir_ptr");

                                } else {

                                    const int deeper_new_dir_ptr_fd = openat(dirfd_destination, entry->d_name, O_RDONLY | O_DIRECTORY);

                                    if (deeper_old_dir_ptr_fd < 0) { // checking file descriptor for errors

                                        exit_code = errno;
                                        perror("openat deeper_new_dir_ptr_fd");

                                    } else {

                                        DIR * deeper_new_dir_ptr = fdopendir(deeper_new_dir_ptr_fd); // getting dir pointer of deeper dir program will copy to

                                        if (deeper_new_dir_ptr == NULL) { // checking work of open for errors

                                            exit_code = errno;
                                            perror("opendir deeper_new_dir_ptr");

                                        } else {

                                            if (copying_dir_content_recursively(deeper_old_dir_ptr, deeper_new_dir_ptr) != 0) {

                                                exit_code = errno;
                                                fprintf(stderr, "Failed to copy %s recursively\n", entry->d_name);

                                            }

                                        }

                                    }

                                }

                            }

                        }

                    }
                }

             }

        }



        

        if (closedir(deeper_old_dir_ptr) != 0)/* closing descriptor with error checking */ 
            exit_code = errno;
            perror("closedir deeper_old_dir_ptr");

        if (closedir(deeper_new_dir_ptr) != 0)
            exit_code = errno;
            perror("closedir deeper_new_dir_ptr");
        
    }
    
    return exit_code;
}

//-----------------------------------------------------------------------------------------------------------------------