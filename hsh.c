#include "hsh.h"

int main(int argc, char *argv[], char *envp[])
{
	struct st_vars vars;

	init_vars(&vars, argv + 1, envp, NULL, NULL, NULL, 0, 0);

	if (!isatty(FILENO_STDIN))
	{
		get_next_line(&vars);
		tokenize(&vars);
		call(&vars);
	}
	else if (argc > 1)
		call(&vars);
	else
	{
		signal(SIGINT, SIG_IGN);
		while (1)
		{
			print_prompt("Myshell$ ");
			get_next_line(&vars);
			tokenize(&vars);
			call(&vars);
		}
	}
	exit_and_free(&vars);
}

void init_vars(struct st_vars *vars,
               char **command,
	       char **envp;
               char *lineptr,
               char *vars,
               alias_t *aliases,
               ssize_t linesize,
               ssize_t commandcount)
{
	vars->command = command;
	vars->env = copyenv(envp);
	vars->lineptr = lineptr;
	vars->vars = vars;
	vars->aliases = aliases;
	vars->linesize = linesize;
	vars->commandcount = commandcount;
	if (hsh_setenv(vars, "PS1", "$ ", 1) == -1)
		if (errno == ENOMEM)
			exit_and_free(vars);
	vars->envsize = countenv(vars->env);
}
