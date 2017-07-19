#include "hsh.h"

void handler(int sig)
{
	hsh_putchar('\n');
	print_prompt();
	fflush(stdout);
}

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
