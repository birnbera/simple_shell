#include "hsh.h"

/**
 * printerror - utility function to print the error message associated
 * with the currently set `errno'
 * @state: struct containing shell state variables
 * @msg: prefix to print before error string
 */
void printerror(struct hsh_state *state, const char * const msg)
{
	perror(msg);
}
