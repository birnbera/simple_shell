#include "hsh.h"

void printerror(struct hsh_state *state, const char * const msg)
{
	perror(msg);
}
