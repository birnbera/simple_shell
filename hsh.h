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
#include <string.h>
#include <ctype.h>

typedef struct env env_t;
typedef struct builtin builtin_t;
typedef struct var var_t;
typedef struct alias alias_t;

struct hsh_state {
	char **command; /* needs to be freed */
	env_t *env; /* needs to be freed */
	char **argv;
	size_t envsize;
	char *lineptr; /* needs to be freed */
	size_t linesize;
	builtin_t **builtins; /* needs to be freed */
	var_t *vars; /* needs to be freed */
	alias_t *aliases; /* needs to be freed */
	size_t commandcount;
	int status;
};

struct env {
	struct env *next;
	char *name;
	char *value;
};

struct alias {
	struct alias *next;
	char *alname;
	char *alvalue;
};

struct var {
	struct var *next;
	char *varname;
	char *varval;
};

struct builtin {
	char *name;
	void (*func)(struct hsh_state *);
};

void print_prompt(void);
void printerror(struct hsh_state *, const char * const msg);
void get_next_line(struct hsh_state *);
char *hsh_strtok(char *, const char *);
void tokenize(struct hsh_state *);
int hsh_setenv(struct hsh_state *, const char *, const char *, int);
char *hsh_getenv(struct hsh_state *, const char *);
env_t *create_venv(const char *, const char *);
void hsh_env(struct hsh_state *);
size_t countenv(env_t *);
env_t *copyenv(struct hsh_state *, char **);
char **env_to_arr(struct hsh_state *);
void search_and_exec(struct hsh_state *);
int check_builtin(struct hsh_state *);
int check_path(struct hsh_state *);
void init_state(struct hsh_state *state, char **argv, char **envp);
void exit_and_free(struct hsh_state *);
void free_command(struct hsh_state *);
void free_env(struct hsh_state *);
void free_lineptr(struct hsh_state *);
void free_builtins(struct hsh_state *);
void free_vars(struct hsh_state *);
void free_aliases(struct hsh_state *);
void hsh_puts(const char * const);
void hsh_putchar(const char);

#endif /* _HSH_H_ */
