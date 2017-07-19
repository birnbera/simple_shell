#include "hsh.h"

void search_and_exec(struct hsh_state *state)
{
	if (check_builtin(state) == 0)
		return;
	if (check_path(state) == 0)
		return;
	printerror(state, "");
}

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

int check_path(struct hsh_state *state)
{
	pid_t child;
	char full_path[1024];
	char **envp, *dir, *path_copy, *path = hsh_getenv(state, "PATH");
	char **command = state->command;

	if (!command || !command[0])
		return (0);
	if (path == NULL)
		path = "";
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		errno = ENOMEM;
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
			if ((child = fork()) < 0)
			{
				printerror(state, "fork");
			}
			if (child == 0)
			{
				envp = env_to_arr(state);
				if (execve(full_path, state->command, envp))
				{
					free(envp);
					free(path_copy);
					state->status = errno;
					printerror(state, "execve");
					exit_and_free(state);
				}
			}
			if (child > 0)
			{
				wait(&state->status);
				return (0);
			}
		}
		dir = hsh_strtok(NULL, ":");
	}
	errno = ENOENT;
	return (-1);
}
