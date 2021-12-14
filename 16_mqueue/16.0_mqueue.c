// to run link with -lrt
#include "../Headers/err_handle.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>

int main (int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage: %s /queuename\n", argv[0]);
		return 1;
	}
	// create a queue or open existing one to both send and receive messages
	mqd_t queue = mq_open(argv[1], O_RDWR | O_CREAT , 0600, NULL);
	if (queue == (mqd_t) -1)
		handle_error("mq_open");


	// send some test message:
	if (mq_send(queue, "Hi!", strlen("Hi!"), 0) == -1) 
		handle_error("mq_send");

	if (mq_send(queue, "The Matrix has you", strlen("The Matrix has you"), 0) == -1) 
		handle_error("mq_send");	

	// get and show queue info:
	struct mq_attr queue_info = {};
	mq_getattr(queue, &queue_info);

	printf("Flags: %ld\n", queue_info.mq_flags);
	printf("Max. number of messages on queue: %ld\n", queue_info.mq_maxmsg);
	printf("Max. message size ()bytes]: %ld\n", queue_info.mq_msgsize);
	printf("Number of messages currently in queue: %ld\n", queue_info.mq_curmsgs);


	// cleanup 	
	mq_close(queue);
	mq_unlink(argv[1]);

	return 0;
}