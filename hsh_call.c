#include "hsh.h"

/**
 * search_and_exec - routine to search for appropriate command to
 * call, first in the builtins, then using the `PATH' variable
 * @state: struct containing shell state variables
 */
void search_and_exec(struct hsh_state *state)
{
	if (check_builtin(state) == 0)
		return;
	if (check_path(state) == 0)
		return;
	printerror(state, "");
}

/**
 * check_builtin - subroutine to check whether current command is
 * a builtin.
 * @state: struct containing shell state variables
 *
 * Return: 0 if command is a builtin and was successfully called,
 * 0 otherwise.
 */
int check_builtin(struct hsh_state *state)
{
	builtin_t **builtins = state->builtins;
	char **command = state->command;
	size_t i;

	for (i = 0; command && *command && builtins && builtins[i]->name; i++)
	{
		if (strcmp(command[0], builtins[i]->name) == 0)
		{
			builtins[i]->func(state);
			return (0);
		}
	}
	return (-1);
}

/**
 * check_path - subroutine to check whether current command can be
 * found in the search path.
 * @state: struct containing shell state variables
 *
 * Return: 0 if command is an executible and is successfully executed,
 * -1 otherwise.
 */
int check_path(struct hsh_state *state)
{
	char full_path[1024];
	char *dir, *path_copy, *path = hsh_getenv(state, "PATH");
	char **command = state->command;

	if (!command || !command[0])
		return (0);
	if (path == NULL)
		path = "";
	if (command[0][0] == '/')
	{
		call(state, command[0]);
		return (0);
	}
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		printerror(state, "malloc");
		exit_and_free(state);
	}
	dir = hsh_strtok(path_copy, ":");
	while (dir)
	{
		strcpy(full_path, dir);
		if (full_path[strlen(full_path) - 1] != '/')
			strcat(full_path, "/");
		strcat(full_path, state->command[0]);
		if (access(full_path, F_OK) == 0)
		{
			call(state, full_path);
			return (0);
		}
		dir = hsh_strtok(NULL, ":");
	}
	free(path_copy);
	errno = ENOENT;
	return (-1);
}

/**
 * call - subroutine to call current command in a subprocess
 * @state: struct containing shell state variables
 * @full_path: complete path to the executible if it exists
 */
void call(struct hsh_state *state, const char *full_path)
{
	char **envp;
	pid_t child = fork();

	if (child < 0)
		printerror(state, "fork");
	if (child == 0)
	{
		envp = env_to_arr(state);
		if (execve(full_path, state->command, envp))
		{
			free(envp);
			printerror(state, "execve");
			exit_and_free(state);
		}
	}
	if (child > 0)
		wait(&state->status);
}
