#ifndef _HSH_H_
#define _HSH_H_

typedef struct free_list_node {
	struct free_list_node *next;
	char **freeptrptr;
} free_list_t;

extern free_list_t *free_list_head;

void print_prompt(void);
char *get_next_line(void);
void exit_and_free(int);
void append_to_free_list(char **);
int del_from_free_list(char **);

#endif /* _HSH_H_ */
