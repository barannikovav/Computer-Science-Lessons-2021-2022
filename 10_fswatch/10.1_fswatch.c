#include <stdio.h>
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include <libfswatch/c/libfswatch.h>
#include "./libfswatch/src/libfswatch/c/libfswatch.h"

#define BUF_SIZE 4096

enum error_codes
{
  ERR_WRONG_ARG_NUM = 1,    // Error code 1 - wrong number of program arguments
  ERR_INITIALIZING_LIBRARY, // Error code 2 - error in initializing fswatch library
  ERR_INITIALIZING_SESSION, // Error code 3 - error in initializing fswatch session
  ERR_ADDING_PATH,          // Error code 4 - error in adding path
  ERR_SETTING_CALLBACK,     // Error code 5 - error in setting callback
  ERR_STARTING_MONITOR,     // Error code 6 - error in starting monitor
  ERR_SIGACTION,            // Error code 7 - error in sigaction
  ERR_THREAD,               // Error code 8 - error in thread work

};

void *thread_stop(void * ptr_arg) {
  FSW_HANDLE *handle = (FSW_HANDLE *) ptr_arg;

  struct pollfd pfds[1];      // creating struct for poll use 
  pfds[0].fd = STDIN_FILENO;  //setting struct for checking stdin
  pfds[0].events = POLLIN;
  int poll_num = 0;

  while (1) {

    poll_num = poll(pfds, 1, -1); // getting events status

    if (poll_num == -1) {
      if (errno == EINTR)
        continue;
      perror("poll");
      return NULL;
    }

    if (poll_num > 0) { // if event happened
      char input_buf[BUF_SIZE];

      if (read(STDIN_FILENO, input_buf, sizeof(input_buf)) > 0 && input_buf[0] == '\n') {

        printf("Stop monitoring path? (Y/N)\n");
        if (read(STDIN_FILENO, input_buf, sizeof(input_buf)) < 0) {
          perror("read");
        }

        if (input_buf[0] == 'Y' || input_buf[0] == 'y') {
          int result = 0; 

          if (fsw_stop_monitor(*handle) != FSW_OK) {
            fprintf(stderr, "Failed to stop monitor. Error: %d\n", fsw_last_error());
            ++result;
          }

          sleep(1);

          if (fsw_destroy_session(*handle) != FSW_OK) {
            fprintf(stderr, "Failed to destroy session. Error: %d\n", fsw_last_error());
            ++result;
          }

          if (result == 0)
            puts("Monitoring stopped");

          break;
        }
      }
    }
  }

  return NULL;
}

void my_callback (fsw_cevent const *const events, 
                  const unsigned int event_num, 
                  void * data) {
  data++;
  data--;
  printf("[%d %s occured:]\n\n", event_num, event_num > 1 ? "events" : "event");

  for (unsigned int i = 0; i < event_num; ++i) {
    printf("Path:            %s\n", events[i].path);
    printf("Trigger time:    %s", ctime(&events[i].evt_time));
    printf("Flags:           ");
    unsigned int flags_number = events[i].flags_num;
    putchar('|');
    for (unsigned int j = 0; j < flags_number; ++j) {
      printf(" %s |", fsw_get_event_flag_name(events[i].flags[j]));
    }

    printf("\n------------------------------------------------------------------------------------------------------------------\n");
  }
  printf(" \n");

  return;
}

int main (int argc, char *argv[]) {

  if (argc != 2) {
    printf("Usage: %s <path> \n", argv[0]);
    return ERR_WRONG_ARG_NUM;
  }

  pthread_t start_thread;

  FSW_STATUS ret = fsw_init_library();
  if (ret != FSW_OK) {
    return ERR_INITIALIZING_LIBRARY;
  }

  FSW_HANDLE handle = fsw_init_session(fsevents_monitor_type);
  if (handle == NULL) {
    fprintf(stderr, "Invalid fswatch handle. Error: %d\n", fsw_last_error());
    return ERR_INITIALIZING_SESSION;
  }

  if (pthread_create(&start_thread, NULL, &thread_stop, (void *) &handle)) {
    perror("Error creating thread");
    return ERR_THREAD;
  }

  if (fsw_add_path(handle, argv[1]) != FSW_OK) {
    fprintf(stderr, "Failed to add path. Error: %d\n", fsw_last_error());
    return ERR_ADDING_PATH;
  }

  if (fsw_set_callback(handle, my_callback, NULL) != FSW_OK) {
    fprintf(stderr, "Failed to set callback. Error: %d\n", fsw_last_error());
    return ERR_SETTING_CALLBACK;
  }

  if (fsw_start_monitor(handle) != FSW_OK) {
    fprintf(stderr, "Failed to start monitor. Error: %d\n", fsw_last_error());
    return ERR_STARTING_MONITOR;
  }

  pthread_join(start_thread, NULL);
  

  return 0;
}