/*
 * tsh - A tiny shell program with job control
 */
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Misc manifest constants */
#define MAXLINE 1024   /* max line size */
#define MAXARGS 128    /* max args on a command line */
#define MAXJOBS 16     /* max jobs at any point in time */
#define MAXJID 1 << 16 /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/*
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;   /* defined in libc */
char prompt[] = "tsh> "; /* command line prompt (DO NOT CHANGE) */
int verbose = 0;         /* if true, print additional output */
int nextjid = 1;         /* next job ID to allocate */
char sbuf[MAXLINE];      /* for composing sprintf messages */

struct job_t {           /* The job struct */
  pid_t pid;             /* job PID */
  int jid;               /* job ID [1, 2, ...] */
  int state;             /* UNDEF, BG, FG, or ST */
  char cmdline[MAXLINE]; /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */

/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
bool builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
bool parseline(const char *cmdline, char **argv);
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs);
bool addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
bool deletejob(struct job_t *jobs, pid_t pid);
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid);
int pid2jid(pid_t pid);
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine
 */
int main(int argc, char **argv) {
  char c;
  char cmdline[MAXLINE];
  int emit_prompt = 1; /* emit prompt (default) */

  /* Redirect stderr to stdout (so that driver will get all output
   * on the pipe connected to stdout) */
  dup2(1, 2);

  /* Parse the command line */
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
    case 'h': /* print help message */
      usage();
      break;
    case 'v': /* emit additional diagnostic info */
      verbose = 1;
      break;
    case 'p':          /* don't print a prompt */
      emit_prompt = 0; /* handy for automatic testing */
      break;
    default:
      usage();
    }
  }

  /* Install the signal handlers */

  /* These are the ones you will need to implement */
  Signal(SIGINT, sigint_handler);   /* ctrl-c */
  Signal(SIGTSTP, sigtstp_handler); /* ctrl-z */
  Signal(SIGCHLD, sigchld_handler); /* Terminated or stopped child */

  /* This one provides a clean way to kill the shell */
  Signal(SIGQUIT, sigquit_handler);

  /* Initialize the job list */
  initjobs(jobs);

  /* Execute the shell's read/eval loop */
  while (1) {

    /* Read command line */
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      app_error("fgets error");
    if (feof(stdin)) { /* End of file (ctrl-d) */
      fflush(stdout);
      exit(0);
    }

    /* Evaluate the command line */
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  }

  exit(0); /* control never reaches here */
}

/*
 * eval - Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
 */
void eval(char *cmdline) {
  char *argv[MAXARGS];
  for (size_t i = 0; i < MAXARGS; i++) {
    argv[i] = NULL;
  }

  bool bg = parseline(cmdline, (char **)argv);

  if (*argv == NULL) {
    return;
  }

  if (builtin_cmd((char **)argv)) {
    return;
  }

  sigset_t full_sigset, child_sigset, old_sigset;
  int result;

  if ((result = sigfillset(&full_sigset)) == -1) {
    unix_error("sigfillset error");
  }
  if ((result = sigemptyset(&child_sigset)) == -1) {
    unix_error("sigemptyset error");
  }
  if ((result = sigaddset(&child_sigset, SIGCHLD)) == -1) {
    unix_error("sigaddset error");
  }

  // Block SIGCHLD signals before forking to ensure addjob is called before
  // deletejob (signal handler will reap the child)
  if ((result = sigprocmask(SIG_BLOCK, &child_sigset, &old_sigset)) == -1) {
    unix_error("sigprocmask error");
  }

  pid_t pid;

  if ((pid = fork()) == -1) {
    unix_error("fork error");
  }

  // child process
  if (pid == 0) {
    if ((result = sigprocmask(SIG_SETMASK, &old_sigset, NULL)) == -1) {
      unix_error("sigprocmask error");
    }

    // Set child's group ID to its PID, so that signals sent to the parent shell
    // process are not recieved by the child
    if (setpgid(0, 0) == -1) {
      unix_error("setpgid error");
    }

    execve(argv[0], argv, NULL);

    printf("%s: Command not found\n", argv[0]);
    exit(0); // execve does not return on success
  }

  // parent process
  if ((result = sigprocmask(SIG_BLOCK, &full_sigset, NULL)) == -1) {
    unix_error("sigprocmask error");
  }
  if (!addjob(jobs, pid, bg ? BG : FG, cmdline)) {
    app_error("Failed to add job");
  }
  if ((result = sigprocmask(SIG_SETMASK, &old_sigset, NULL)) == -1) {
    unix_error("sigprocmask error");
  }

  if (bg) {
    printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
  } else {
    waitfg(pid);
  }

  return;
}

/*
 * parseline - Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
bool parseline(const char *cmdline, char **argv) {
  static char array[MAXLINE]; /* holds local copy of command line */
  char *buf = array;          /* ptr that traverses command line */
  char *delim;                /* points to first space delimiter */
  int argc;                   /* number of args */
  int bg;                     /* background job? */

  strcpy(buf, cmdline);
  buf[strlen(buf) - 1] = ' ';   /* replace trailing '\n' with space */
  while (*buf && (*buf == ' ')) /* ignore leading spaces */
    buf++;

  /* Build the argv list */
  argc = 0;
  if (*buf == '\'') {
    buf++;
    delim = strchr(buf, '\'');
  } else {
    delim = strchr(buf, ' ');
  }

  while (delim) {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (*buf && (*buf == ' ')) /* ignore spaces */
      buf++;

    if (*buf == '\'') {
      buf++;
      delim = strchr(buf, '\'');
    } else {
      delim = strchr(buf, ' ');
    }
  }
  argv[argc] = NULL;

  if (argc == 0) /* ignore blank line */
    return true;

  /* should the job run in the background? */
  if ((bg = (*argv[argc - 1] == '&')) != 0) {
    argv[--argc] = NULL;
  }
  return bg;
}

/*
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.
 */
bool builtin_cmd(char **argv) {
  assert(argv != NULL && "argv is NULL");

  if (*argv == NULL) {
    return false;
  }

  char *cmd = argv[0];
  size_t cmd_len = strnlen(cmd, MAXLINE);

  if (strncmp(cmd, "quit", cmd_len) == 0 ||
      strncmp(cmd, "exit", cmd_len) == 0) {
    // I'm going to assume we don't need to kill background jobs
    exit(0);
  }

  if (strncmp(cmd, "jobs", cmd_len) == 0) {
    listjobs(jobs);
    return true;
  }

  if (strncmp(cmd, "bg", cmd_len) == 0) {
    printf("builtin_cmd 'bg' not implemented\n");
    return true;
  }

  if (strncmp(cmd, "fg", cmd_len) == 0) {
    printf("builtin_cmd 'fg' not implemented\n");
    return true;
  }

  return false; /* not a builtin command */
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) {
  return;
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
  while (fgpid(jobs) == pid) {
    // TODO: can we use sigsuspend instead?
    sleep(1);
  }
}

/*****************
 * Signal handlers
 *****************/

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.
 */
void sigchld_handler(int sig) {
  int old_errno = errno;
  sigset_t full_sigset, old_sigset;
  pid_t pid;
  int result;

  // NOTE: write() used for error messages as printf and friends are not
  // async-signal-safe and should not be used in signal handlers

  // NOTE: constant values for error message lengths used as strlen is not
  // async-signal-safe (compiler warnings will catch if the msg_len is to small
  // for the string literal)

  // NOTE: _exit() used as exit() is not async-signal-safe

  if ((result = sigfillset(&full_sigset)) == -1) {
    const char msg[17] = "sigfillset error\n";
    result = write(STDERR_FILENO, msg, 17);
    _exit(EXIT_FAILURE);
  }

  int wait_for_pid = -1; // wait for any child process
  int wstatus;
  const int options = WNOHANG | WUNTRACED;
  // WNOHANG: return immediately if no child has exited.
  // WUNTRACED: also return if a child has stopped

  while ((pid = waitpid(wait_for_pid, &wstatus, options)) > 0) {
    if ((result = sigprocmask(SIG_BLOCK, &full_sigset, &old_sigset)) == -1) {
      const char msg[18] = "sigprocmask error\n";
      result = write(STDERR_FILENO, msg, 18);
      _exit(EXIT_FAILURE);
    }

    if (WIFSTOPPED(wstatus)) {
      // TODO test08: make sure SIGSTP is being handled correctly
      // Update state of pid job to be ST
      const char msg[46] = "[sigchld_handler]: WIFSTOPPED not implemented\n";
      result = write(STDERR_FILENO, msg, 46);
      _exit(EXIT_FAILURE);
    } else if (!deletejob(jobs, pid)) {
      const char msg[21] = "Failed to delete job\n";
      result = write(STDERR_FILENO, msg, 21);
      _exit(EXIT_FAILURE);
    }

    if ((result = sigprocmask(SIG_SETMASK, &old_sigset, NULL)) == -1) {
      const char msg[18] = "sigprocmask error\n";
      result = write(STDERR_FILENO, msg, 18);
      _exit(EXIT_FAILURE);
    }
  }

  errno = old_errno;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig) {
  return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig) {
  return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
  job->pid = 0;
  job->jid = 0;
  job->state = UNDEF;
  job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++)
    clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) {
  int i, max = 0;

  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid > max)
      max = jobs[i].jid;
  return max;
}

/* addjob - Add a job to the job list */
bool addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) {
  int i;

  if (pid < 1)
    return false;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == 0) {
      jobs[i].pid = pid;
      jobs[i].state = state;
      jobs[i].jid = nextjid++;
      if (nextjid > MAXJOBS)
        nextjid = 1;
      strcpy(jobs[i].cmdline, cmdline);
      if (verbose) {
        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid,
               jobs[i].cmdline);
      }
      return true;
    }
  }
  printf("Tried to create too many jobs\n");
  return false;
}

/* deletejob - Delete a job whose PID=pid from the job list */
bool deletejob(struct job_t *jobs, pid_t pid) {
  int i;

  if (pid < 1)
    return false;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == pid) {
      clearjob(&jobs[i]);
      nextjid = maxjid(jobs) + 1;
      return true;
    }
  }
  return false;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].state == FG)
      return jobs[i].pid;
  return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
  int i;

  if (pid < 1)
    return NULL;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid)
      return &jobs[i];
  return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) {
  int i;

  if (jid < 1)
    return NULL;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid == jid)
      return &jobs[i];
  return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) {
  int i;

  if (pid < 1)
    return 0;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid) {
      return jobs[i].jid;
    }
  return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid != 0) {
      printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
      switch (jobs[i].state) {
      case BG:
        printf("Running ");
        break;
      case FG:
        printf("Foreground ");
        break;
      case ST:
        printf("Stopped ");
        break;
      default:
        printf("listjobs: Internal error: job[%d].state=%d ", i, jobs[i].state);
      }
      printf("%s", jobs[i].cmdline);
    }
  }
}
/******************************
 * end job list helper routines
 ******************************/

/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) {
  printf("Usage: shell [-hvp]\n");
  printf("   -h   print this message\n");
  printf("   -v   print additional diagnostic information\n");
  printf("   -p   do not emit a command prompt\n");
  exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg) {
  fprintf(stdout, "%s: %s\n", msg, strerror(errno));
  exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg) {
  fprintf(stdout, "%s\n", msg);
  exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) {
  struct sigaction action, old_action;

  action.sa_handler = handler;
  sigemptyset(&action.sa_mask); /* block sigs of type being handled */
  action.sa_flags = SA_RESTART; /* restart syscalls if possible */

  if (sigaction(signum, &action, &old_action) < 0)
    unix_error("Signal error");
  return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) {
  printf("Terminating after receipt of SIGQUIT signal\n");
  exit(1);
}
