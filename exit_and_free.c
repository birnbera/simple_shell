#include "hsh.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

free_list_t *free_list_head = NULL;

void exit_and_free(int status)
{
	free_list_t *next, *head = free_list_head;

	while (head)
	{
		next = head->next;
		free(*(head->freeptrptr));
		free(head);
		head = next;
	}
	exit(status);
}

void append_to_free_list(char **ptr)
{
	free_list_t *new, *next = free_list_head;

	new = malloc(sizeof(free_list_t));
	if (new == NULL)
	{
		perror("malloc");
		exit_and_free(errno);
	}
	new->freeptrptr = ptr;
	new->next = NULL;
	if (next == NULL)
	{
		free_list_head = new;
		return;
	}
	while (next->next)
		next = next->next;
	next->next = new;
}

int del_from_free_list(char **ptr)
{
	free_list_t *prev, *curr, *next;

	if (free_list_head == NULL)
	{
		dprintf(2, "No elements to free\n");
		return (-1);
	}
	prev = free_list_head;
	while ((curr = prev->next))
	{
		next = curr->next;
		if (*(curr->freeptrptr) == *ptr)
		{
			prev->next = next;
			free(*(curr->freeptrptr));
			free(curr);
			return (0);
		}
	}
	dprintf(2, "Pointer to free not found\n");
	return (-1);
}
