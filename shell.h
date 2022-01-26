#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

#define SUCCESSFUL 0
#define ERROR -1

typedef struct {
    char *path;	        // full path to the executable or built-in cmd
    int argc;	        // total number of arguments
    char **argv;	// argument values
} command_t;

void parse(char *line, command_t *p_cmd);

bool find_fullpath(command_t *p_cmd);

int execute(command_t *p_cmd);

bool is_builtin(command_t *p_cmd);

int do_builtin(command_t *p_cmd);

void cleanup(command_t *p_cmd);

#endif // SHELL_H