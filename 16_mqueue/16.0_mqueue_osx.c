#include <stdio.h>
#include <sys/ipc.h>
#include "../Headers/err_handle.h"

#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MSG_SIZE 20

int main(int argc, char *argv[]) {
  
  if (argc != 2) {
    printf("Usage: %s /queuekey \n", argv[0]);
    return 1;
  }


  // get a message queue id
  key_t key = atoi(argv[1]);
  int msqid = msgget(key, IPC_CREAT | 0600);
  if (msqid == -1)
      handle_error("msgget");

  struct message { // creating struct for sending a message
    long type;
    char text[MSG_SIZE];
  } msg;

  // send some test message
  msg.type = 1;
  strcpy(msg.text, "Hi!");
  if (msgsnd(msqid, (void *) &msg, sizeof(msg.text), IPC_NOWAIT) == -1) // sending message with IPC_NOWAIT flag so, if the mqueue is full,
      handle_error("msgsnd");                                           // control will be returned to calling process without block

  // get message queue data structure
  struct msqid_ds buf;
  if (msgctl(msqid, IPC_STAT, &buf) == -1)
      handle_error("msgget");

  printf("Mode:                                    %ho\n", buf.msg_perm.mode);  
  printf("Number of messages currently on queue:   %ld\n", buf.msg_qnum);
  printf("Max. number of bytes allowed on queue:   %ld\n", buf.msg_qbytes);
  printf("Process ID of last msgsnd():             %d\n", buf.msg_lspid);
  printf("Process ID of last msgrcv():             %d\n", buf.msg_lrpid);
  printf("Time of last msgsnd():                   %s", ctime(&buf.msg_stime));
  printf("Time of last msgrcv():                   %s", ctime(&buf.msg_rtime));
  printf("Time of last change:                     %s", ctime(&buf.msg_ctime));

  if (msgctl(msqid, IPC_RMID, NULL) == -1) // cleanup
      handle_error("msgget");

  return 0;
}
