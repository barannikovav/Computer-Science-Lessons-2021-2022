#include "../Headers/err_handle.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

void proc_info (const char * procname) { /* function for displaying info about process */
	
	printf(
					"%s: PID %d, PPID %d, PGID %d, SID %d\n",
					procname, getpid(), getppid(), getpgid(0), getsid(0)
	);
}

int main (void) {
	pid_t child_id = fork(); /* creating child proces with fork call */
	
	if (child_id < 0) /* checking for errors */
		handle_error("fork");

	if (child_id == 0) { /* this part of code will be executed only in a child process */
		proc_info("Child");

		pid_t parent_pid = getppid();

		if (ptrace(PT_ATTACHEXC, parent_pid, 0, 0) < 0)
			handle_error("ptrace child");

		int status = 0;
		pid_t res = 0;

		while(1) {

			res = waitpid(-1, &status, WUNTRACED | WCONTINUED);
			if (res < 0)
				handle_error("waitpid");

			if (WIFEXITED(status))
				printf("Parent with PID = %d terminated normally by a call to <<exit>>. Exit status: %d\n", res, WEXITSTATUS(status)); break;
			if (WIFSTOPPED(status))
				printf("Parent with PID = %d has stopped by signal %d (%s) but can be restarted\n", res, WSTOPSIG(status), strsignal(WSTOPSIG(status)));
			if (WIFCONTINUED(status))
				printf("Parent with PID = %d has been continued\n", res);
			
			if (WIFSIGNALED(status)) {

				int child_signal = WTERMSIG(status);
				printf("Parent with PID = %d was terminated due to reсeipt of signal %d (%s)\n", res, child_signal, strsignal(child_signal));

				if (WCOREDUMP(status))
					printf("Parent with PID = %d has caused core dump\n", res);

				break;

		}

	} 

	} else {
		proc_info("Parent");

		sleep(20);

	}

	return 0;
}

