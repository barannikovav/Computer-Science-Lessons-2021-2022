#define _GNU_SOURCE

#include <stdio.h>
#include <dirent.h> /* defines DT_* constants */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h> // to return errno
#include <fcntl.h>

// function for convenient error handling
#define handle_error(msg) \
				do { int errno_code = errno; perror(msg); return errno_code; } while (0)

//-----------------------------------------------------------------------------------------------------------------------

struct linux_dirent64 /* the structure with which dirent_64 works */
{
    ino64_t        d_ino;    /* 64-bit inode number */
    off_t          d_off;    /* 64-bit offset to next structure */
    unsigned short d_reclen; /* Size of this dirent */
    unsigned char  d_type;   /* File type */
    char           d_name[]; /* Filename (null-terminated) */
};

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

#define BUF_SIZE 1024
int main (int argc, char *argv[]) {
	int fd, nread;
  char buf[BUF_SIZE];
  struct linux_dirent *d;
  int bpos;
  char d_type_l;

  fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY); /* getting fd descriptor */
  if (fd == -1)                                                /* O_RDONLY - read-only perms */
  	handle_error("open");                                      /* O_DIRECTORY - if pathname is not a dir, cause open to fail */

  while (1) { /* working until error or end of dir */
  	nread = getdents64(fd, buf, BUF_SIZE);

  	if (nread == -1)
  		handle_error("getdents64");
  	if (nread == 0)
  		break;
  	printf("--------------- nread=%d ---------------\n", nread);
    printf("inode#    file type  d_reclen  d_off   d_name\n");
    for (bpos = 0; bpos < nread; ) { /* reading info from buf until end of it */
    	d = (struct linux_dirent64 *) (buf + bpos); /* getting right struct ptr from buf */
    	printf("%8ld  ", d->d_ino);

    	d_type_l = dtype_char(d->d_type); /* getting file type with dtype */
      if (d_type_l == '?') {            /* in case of unknown type/error one more check with stat */
    		struct stat sb;
				if (fstat(fd, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW) < 0) 
				handle_error("fstatat");
				else 
				d_type_l = mode_char(sb.st_mode);
		}
		printf("%-10s  %4d %10lld  %s\n", d_type_l, d->d_reclen, (long long) d->d_off, d->d_name);
		bpos += d->d_reclen;
    }

  }

  close(fd);

  return 0;
}