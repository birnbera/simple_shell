#ifndef _HSH_H_
#define _HSH_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

typedef struct env {
	struct env *next;
	char *name;
	char *value;
} env_t;

struct st_vars {
	char **command;
	env_t *env;
	size_t envsize;
	char *lineptr;
	char *vars;
	alias_t *aliases;
	ssize_t linesize;
	ssize_t commandcount;
};

void init_vars(struct st_vars *,
	       char **command,
	       ent_t *env,
	       char *lineptr,
	       char *vars,
	       alias_t *aliases,
	       ssize_t linesize,
	       ssize_t commandcount);
void print_prompt(struct st_vars *);
void get_next_line(struct st_vars *);
void exit_and_free(int, struct st_vars *);
char *hsh_strtok(char *, const char *);
int hsh_setenv(struct st_vars *, const char *, const char *, int);
char *hsh_getenv(struct st_vars *, const char *);
env_t *create_venv(const char *, const char *);
void hsh_env(struct st_vars *);
size_t countenv(env_t *);
env_t *copyenv(char **);
char **env_to_arr(struct st_vars *);

#endif /* _HSH_H_ */
