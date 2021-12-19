// NOTE. IF YOU USE THIS PROGRAM WITH LINUX KERNEL, UNCOMMENT LINUX_KERNEL FOR MORE INFO

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sched.h>

#include "../Headers/err_handle.h"

#define DEFAULT_BUF_SIZE 16
//#define LINUX_KERNEL

int main (void) {
  
  struct passwd *pwd;
  struct group *grp;

  printf("------------------------------------[ORDINARY INFO]--------------------------------------\n\n");

  printf("Process id: %d\n", getpid());
  printf("Process parent id: %d\n", getppid());

  pwd = getpwuid(getuid());

  if (pwd == NULL) 
    handle_error("getpwuid");
  
  printf("User id: %d\n", pwd->pw_uid);
  printf("User name: %s\n", pwd->pw_name);
  printf("Group id: %d\n", pwd->pw_gid);

  grp = getgrgid(pwd->pw_gid);
  if (grp == NULL)
    handle_error("getgrgid");

  printf("Group name: %s\n\n", grp->gr_name);

  printf("--------------------------------------[GROUP LIST]--------------------------------------\n\n");

  gid_t *grouplist;
  int number_of_groups;
  size_t max_number_of_groups;

  max_number_of_groups = NGROUPS_MAX > 0 ? NGROUPS_MAX : DEFAULT_BUF_SIZE;

  grouplist = (gid_t *)malloc(max_number_of_groups * sizeof(gid_t));

  if (grouplist == NULL) 
    handle_error("malloc");

  number_of_groups = getgroups((int)max_number_of_groups, grouplist); // calling getgroup to get number of groups

  if (getgroups((int) max_number_of_groups, grouplist) == -1) // calling getgroup to get grouplist
    perror("getgroups");

  printf("[List of %d groups]   group id    group name\n", number_of_groups);
  for (int i = 0; i < number_of_groups; i++) {
    grp = getgrgid(grouplist[i]);
    if (grp == NULL)
    handle_error("getgrgid");

    printf("%27d         %s\n", grouplist[i], grp->gr_name);
  }
  putchar('\n');

  struct rlimit rmp;

  printf("-------------------------------------[LIMITS INFO]--------------------------------------\n\n");

  printf("                     LIMITS                                                           SOFT                      HARD               \n\n");
  getrlimit(RLIMIT_CORE, &rmp);
  printf("The largest size (in bytes) core file that may be created                     %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_CPU, &rmp);
  printf("The maximum amount of cpu time (in seconds) to be used by each process        %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_DATA, &rmp);
  printf("The maximum size (in bytes) of the data segment for a process                 %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_FSIZE, &rmp);
  printf("The largest size (in bytes) file that may be created                          %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_MEMLOCK, &rmp);
  printf("The maximum size (in bytes) which a process may lock into memory              %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_NOFILE, &rmp);
  printf("The maximum number of open files for this process                             %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_NPROC, &rmp);
  printf("The maximum number of simultaneous processes for this user id                 %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_RSS, &rmp);
  printf("The maximum size (in bytes) to which a process's resident set size may grow   %19llu       %19llu\n", rmp.rlim_cur, rmp.rlim_max);
  getrlimit(RLIMIT_STACK, &rmp);
  printf("The maximum size (in bytes) of the stack segment for a process                %19llu       %19llu\n\n", rmp.rlim_cur, rmp.rlim_max);


  printf("-------------------------------------[SCHEDULER INFO]--------------------------------------\n\n");
  // unfortunately most part of schedule info is not available on Darwin(OSX) kernel
  #ifdef LINUX_KERNEL 
  int policy = sched_getscheduler(0); // scheduler policy of calling process (pid_t = 0)
  // SCHED_OTHER = 0, SCHED_FIFO = 1,
  // SCHED_RR = 2,


  printf("Policy:                                                           %d\n", policy);

  printf("Priority:                                                         %d\n", getpriority(PRIO_PROCESS, 0));

  if (policy != SCHED_OTHER) { // we exclude this policies, because their priority is always 0
    printf("Maximum priority value that can be used by scheduler:           %d\n", sched_get_priority_max(0));
    printf("Minumum priority value that can be used by scheduler:           %d\n", sched_get_priority_min(0));
  } else if (policy == SCHED_RR) {
    struct timespec quantum;
    if (sched_rr_get_interval(0, &quantum) < 0) 
      perror("sched_rr_get_interval");

    printf("Round Robin quantum:                                            %ld.%09ld\n\n", quantum.tv_sec, quantum.tv_nsec);
  
  }
  #else
  
  printf("Priority:                                                         %d\n", getpriority(PRIO_PROCESS, 0));
  
  printf("Maximum priority value that can be used by scheduler:             %d\n", sched_get_priority_max(0));
  printf("Minumum priority value that can be used by scheduler:             %d\n\n", sched_get_priority_min(0));
  

  #endif


  return 0;
}