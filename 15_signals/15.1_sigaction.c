#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "../Headers/err_handle.h"

struct my_sigaction {
  int signo;
  int err_no;
  pid_t pid;
  uid_t uid;
  int status;
};

volatile int g_last_signal;
volatile struct my_sigaction g_last_signal_info = {}; // creating struct for sigaction; 

void sig_handler(int signum, siginfo_t* info, void* ucontext) {

  g_last_signal = signum;
  g_last_signal_info.signo = info->si_signo;
  g_last_signal_info.err_no = info->si_errno;
  g_last_signal_info.pid = info->si_pid;
  g_last_signal_info.uid = info->si_uid;
  g_last_signal_info.status = info->si_status;

  // this variable is useful in this program
  ucontext = (void*)ucontext;

}

int main (void) {

  int end_code = 0;

  struct sigaction recieved_signal = {};

  recieved_signal.sa_flags = SA_SIGINFO | SA_RESTART; // flag sets using of handler with 3 arguments
  recieved_signal.sa_sigaction = sig_handler;

  for (int i = 1; i < NSIG; i++) {
    if (i != SIGKILL && i != SIGSTOP) {
      if (sigaction(i, &recieved_signal, NULL) < 0) {
        perror("sigaction");
        end_code = -1;
      }
    }
  }

  while (1) {
    pause(); // using pause(), because sigwait() func blocks signal and the handler is not used; for more info check man sigwait(2)

    printf("Signal %d (%s) came from process with parameters: \n", g_last_signal, strsignal(g_last_signal));
    printf("Signal number:                    %d\n", g_last_signal_info.signo);
    printf("Errno:                            %d\n", g_last_signal_info.err_no);
    printf("PID:                              %u\n", g_last_signal_info.pid);
    printf("UID:                              %u\n", g_last_signal_info.uid);
    printf("Exit value or signal:             %d\n", g_last_signal_info.status);

  }

  return end_code;
}