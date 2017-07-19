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

/**
 * env_t - Typedef for environment linked list node
 */
typedef struct env env_t;

/**
 * builtin_t - Typedef for builtin function struct
 */
typedef struct builtin builtin_t;

/**
 * var_t - Typedef for generic shell variable struct
 */
typedef struct var var_t;

/**
 * alias_T - Typedef for command alias struct
 */
typedef struct alias alias_t;

/**
 * struct hsh_state - Struct to hold persistant shell variables and lists
 * @command: command to be passed to `execve'
 * @env: linked list of environment variables
 * @argv: arguments passed to main from initial invocation
 * @envsize: stored size of environment linked list to facilitate operations
 * @lineptr: line of input text returned by `getline'
 * @linesize: number of elements allocated to `lineptr'
 * @builtins: array of structs for each builtin function and its name
 * @vars: pointer to list of generic shell variables
 * @aliases: pointer to list of command aliases
 * @commandcount: number of commands so far executed by the current shell
 * @status: return status of previous command
 * @errno: pointer to currently set errno value
 *
 * Description: This struct contains all the information about the current
 * state of the shell program and is passed from function to function. This
 * not only simplifies keeping track of variables, but also allows functions
 * that have encountered an error to free dynamically allocated memory and exit
 * without returning to the `main' function.
 */
struct hsh_state
{
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
	int *err;
};

/**
 * struct env - node of linked list to hold environment variables
 * @next: pointer to next node in list
 * @name: variable name
 * @value: variable value
 *
 * Description: each variable is parsed from the form `name=value' and stored
 * with `name' and `value' separated for ease of retrieval and use.
 */
struct env
{
	struct env *next;
	char *name;
	char *value;
};

/**
 * struct alias - node of linked list to hold command aliases
 * @next: pointer to next node in list
 * @alname: command alias
 * @alvalue: alias replacement string
 *
 * Description: when set by `alias' builtin using `alias=`replacement string'',
 * the alias is set to `alname' and the replacement to `alvalue'
 */
struct alias
{
	struct alias *next;
	char *alname;
	char *alvalue;
};

/**
 * struct var - node of linked list to contain generic shell variables
 * @next: next node in linked list
 * @varname: name of variable
 * @varval: variable value
 *
 * Description: similar to env but not exported to child processes
 */
struct var
{
	struct var *next;
	char *varname;
	char *varval;
};

/**
 * struct builtin - struct containing name of builtin and function pointer
 * @name: name of builtin command
 * @func: pointer to function taking `struct hsh_state' and returning void
 *
 * Description: struct to store builtin functions and their names using a
 * consistant interface for all calls.
 */
struct builtin
{
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
void exit_wrap(struct hsh_state *);
void free_command(struct hsh_state *);
void free_env(struct hsh_state *);
void free_lineptr(struct hsh_state *);
void free_builtins(struct hsh_state *);
void free_vars(struct hsh_state *);
void free_aliases(struct hsh_state *);
void hsh_puts(const char * const);
void hsh_putchar(const char);
void call(struct hsh_state *, const char *);
void set_builtins(struct hsh_state *);

#endif /* _HSH_H_ */
