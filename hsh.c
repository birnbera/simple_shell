#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
	char *line, **command;

	while (1)
	{
		print_prompt();
		line = get_next_line();
		command = tokenize(line);
		call_command(command);
	}
	free(command);
	free(line);
	exit(EXIT_SUCCESS);
}
