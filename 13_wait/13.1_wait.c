#include "../Headers/err_handle.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

void proc_info (const char * procname) { /* function for displaying info about process */
	
	printf(
					"%s: PID %d, PPID %d, PGID %d, SID %d\n",
					procname, getpid(), getppid(), getpgid(0), getsid(0)
	);
}

int main (int argc, char *argv[]) {
	
	pid_t child_id = fork(); /* creating child proces with fork call */
	
	if (child_id < 0) /* checking for errors */
		handle_error("fork");

	if (child_id == 0) { /* this part of code will be executed only in a child process */
		proc_info("Child");
		if (argc > 1)
			printf("%d\n", 42 / atoi(argv[1]));
		
		sleep(20);

		return 42;
	} else { /* this part of code will be executed only in a parent process */
		proc_info("Parent");
		int status;             /* variable which stores info about child process termination */
		pid_t res = 0;          /* variable which stores result of waitpid work */

		while(1) { /* waiting for change in child process in loop  */
			res = waitpid(-1, &status, WUNTRACED | WCONTINUED); /* getting any child process signal info */
                                                          /* WUNTRACED - children of current process that are stopped due SIG signals have reported too  */
			if (res < 0)                                        /* WCONTINUED - let child processes have been resumed after SIGCONT  */
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
		}
	}

	return 0;
}