#include "../Headers/err_handle.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

//#define LINUX_KERNEL undef if you are using Linux

void proc_info (const char * procname) { /* function for displaying info about process */
  
  printf(
          "%s: PID %d, PPID %d, PGID %d, SID %d\n",
          procname, getpid(), getppid(), getpgid(0), getsid(0)
  );
}

int main (void) {
  pid_t parent_pid = getppid();

  pid_t child_id = fork(); /* creating child proces with fork call */
  
  if (child_id < 0) /* checking for errors */
    handle_error("fork");

  if (child_id == 0) { /* this part of code will be executed only in a child process */
    proc_info("Child");

    #ifdef LINUX_KERNEL
      fd pidfd = pidfd_open(parent_pid, 0);
    #else
    if (ptrace(PT_ATTACHEXC, parent_pid, 0, 0) < 0)
      handle_error("ptrace child");
    #endif

    int status = 0;
    pid_t res = 0;
    
    while(1) {
      #ifdef LINUX_KERNEL
        siginfo_t info;
        res = waitid(P_PIDFD, pidfd, info, WCONTINUED);

        printf("Signal number:                    %d\n", info.si_signo);
        printf("The process ID of the child:      %u\n", info.si_pid);
        printf("The real user ID of the child:    %u\n", info.si_uid);
        printf("Exit value or signal:             %d\n", info.si_status);

        if (info.si_code == CLD_EXITED) {
          printf("Child with PID = %d terminated normally by a call to <<exit>>. Exit status: %d\n", info.si_pid, info.si_status); break;
        } else if (info.si_code == CLD_STOPPED) {
          printf("Child with PID = %d has stopped by signal %d (%s) but can be restarted\n", info.si_pid, info.si_status, strsignal(info.si_status));
        } else if (info.si_code == CLD_CONTINUED) {
          printf("Child with PID = %d has been continued\n", info.si_pid);
        } else if (info.si_code == CLD_KILLED) {
          printf("Child with PID = %d was terminated due to reсeipt of signal %d (%s)\n", info.si_pid, info.si_status, strsignal(info.si_status));

        if (info.si_code == CLD_DUMPED)
          printf("Child with PID = %d has caused core dump\n", info.si_pid);

          break;
        }

        #else

        res = waitpid(parent_pid, &status, WUNTRACED | WCONTINUED);

        if (res < 0)
        handle_error("waitpid");

        if (WIFEXITED(status)) {
          printf("Child with PID = %d terminated normally by a call to <<exit>>. Exit status: %d\n", res, WEXITSTATUS(status)); break;
        } else if (WIFSTOPPED(status)) {
          printf("Child with PID = %d has stopped by signal %d (%s) but can be restarted\n", res, WSTOPSIG(status), strsignal(WSTOPSIG(status)));
        } else if (WIFCONTINUED(status)) {
          printf("Child with PID = %d has been continued\n", res);
        } else if (WIFSIGNALED(status)) {
          int child_signal = WTERMSIG(status);
          printf("Child with PID = %d was terminated due to reсeipt of signal %d (%s)\n", res, child_signal, strsignal(child_signal));

        if (WCOREDUMP(status))
          printf("Child with PID = %d has caused core dump\n", res);

          break;
        }
      #endif
    }

  }  else {
    proc_info("Parent");

    sleep(20);

  }

  return 0;
}

