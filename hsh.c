#include "hsh.h"

/**
 * handler - signal handler to catch SIGINT (ctrl-c) from parent process
 * @sig: value of SIGINT passed from signal trap
 */
void handler(int sig)
{
	(void)(sig);
	hsh_putchar('\n');
	print_prompt();
	fflush(stdout);
}

/**
 * main - outermost routine that implements this simple shell program
 * @argc: number of arguments to main
 * @argv: positional arguments from command-line
 * @envp: array of envronment variables from parent process
 *
 * Return: 0 on success, otherwise a command specific error. This function
 * never returns directly, but calls `exit_and_free' to first free memory.
 */
int main(int argc, char *argv[], char *envp[])
{
	pid_t child;
	struct hsh_state state;

	init_state(&state, argv, envp);
	if (argc > 1)
	{
		child = fork();
		if (child == -1)
			printerror(&state, NULL);
		if (child == 0)
		{
			if (execve(argv[1], argv + 1, envp))
			{
				printerror(&state, NULL);
				exit_and_free(&state);
			}
		}
		if (child > 0)
			wait(&(&state)->status);
		exit_and_free(&state);
	}

	if (isatty(STDIN_FILENO))
		signal(SIGINT, handler);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();
		get_next_line(&state);
		tokenize(&state);
		search_and_exec(&state);
	}
	exit_and_free(&state);
}

/**
 * init_state - function to initialize the state of shell variables
 * upon initial startup
 * @state: struct containing shell state variables
 * @argv: pointer to initially passed command line arguments
 * @envp: pointer to initially passed environment variables
 */
void init_state(struct hsh_state *state, char **argv, char **envp)
{
	state->argv = argv;
	state->env = copyenv(state, envp);
	state->lineptr = NULL;
	state->command = NULL;
	state->vars = NULL;
	state->aliases = NULL;
	state->linesize = 0;
	state->commandcount = 0;
	state->envsize = countenv(state->env);
	state->status = 0;
	state->err = &errno;
	set_builtins(state);
}

/**
 * set_builtins - function to set the array of builtins to the
 * relevant member variable of `state'
 * @state: struct of shell state variables
 */
void set_builtins(struct hsh_state *state)
{
	builtin_t *builtin;
	builtin_t builtins[] = {
		{"exit", exit_wrap},
		{"env", hsh_env},
		{NULL, NULL}
	};
	size_t i;

	state->builtins = malloc(sizeof(builtin_t *) * sizeof(builtins));
	if (state->builtins == NULL)
	{
		printerror(state, "malloc");
		exit_and_free(state);
	}
	for (i = 0; builtins[i].name; i++)
	{
		builtin = malloc(sizeof(builtin_t));
		if (builtin == NULL)
		{
			printerror(state, "malloc");
			exit_and_free(state);
		}
		builtin->name = hsh_strdup(builtins[i].name);
		builtin->func = builtins[i].func;
		state->builtins[i] = builtin;
	}
	builtin = malloc(sizeof(builtin_t));
	if (builtin == NULL)
	{
		printerror(state, "malloc");
		exit_and_free(state);
	}
	builtin->name = NULL;
	builtin->func = NULL;
	state->builtins[i] = builtin;
}
