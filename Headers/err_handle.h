#ifndef ERR_HANDLE
#define ERR_HANDLE

#include <sys/errno.h>

// function for convenient error handling
#define handle_error(msg) \
				do { int errno_code = errno; perror(msg); return errno_code; } while (0)

// function for convenient error handling with freeing
#define handle_error_free(msg, ptr) \
				do { int errno_code = errno; perror(msg); free(ptr); return errno_code; } while (0)

#endif /* ERR_HANDLE */
