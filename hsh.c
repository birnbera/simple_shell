#include "hsh.h"

/**
 * handler - signal handler to catch SIGINT (ctrl-c) from parent process
 * @sig: value of SIGINT passed from signal trap
 */
void handler(int sig)
{
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
	struct hsh_state state;

	init_state(&state, argv, envp);
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
