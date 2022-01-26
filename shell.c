// PID: 730329696
// I pledge the COMP 211 honor code.

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

#include "shell.h"

const char *BUILT_IN_COMMANDS[] = {"cd", "exit", NULL};
const char *PATH_SEPARATOR = ":";

void parse(char *line, command_t *p_cmd){
    char *token;
    int argc = 0;

    while((token = strtok_r(line, " \n\t\r", &line)) != NULL){
        strcpy(p_cmd->argv[argc], token);
        argc++;
    }

    strcpy(p_cmd->path, p_cmd->argv[0]);
    p_cmd->argc = is_builtin(p_cmd) || find_fullpath(p_cmd) ? argc : ERROR;
}

bool find_fullpath(command_t *p_cmd){
    char path[300];
    strcpy(path, getenv("PATH"));

    char *token = strtok(path, PATH_SEPARATOR);

    while(token != NULL){
        struct stat buffer;
        char file_or_dir[300] = "";
        strcat(strcat(strcat(file_or_dir, token), "/"), p_cmd->path);
        int exists = stat(file_or_dir, &buffer);

        if(exists == 0 && (S_IFREG & buffer.st_mode)) {
            strcpy(p_cmd->path, file_or_dir);
            return true;
        }

        token = strtok(NULL, PATH_SEPARATOR);
    }
    return false;
}

int execute(command_t *p_cmd){
    int status = SUCCESSFUL;
    pid_t child_pid = fork();

    if (child_pid == 0) {
        p_cmd->argv[p_cmd->argc] = NULL;
        execv(p_cmd->path, p_cmd->argv);
        status = ERROR;
        perror("Execute terminated with an error condition!\n");
        exit(1);
    }
    wait(NULL);
    return status;
}


bool is_builtin(command_t *p_cmd){
    int i = 0;

    while(BUILT_IN_COMMANDS[i] != NULL){
        if(strcmp(p_cmd->path, BUILT_IN_COMMANDS[i]) == 0){
            return true;
        }

        i++;
    }

    return false;
}


int do_builtin(command_t *p_cmd){
    // For whatever reason, `exit` is handled in driver.c.
    // This just handles `cd`

    struct stat buff;
    int status = ERROR;

    if(p_cmd->argc == 1){
        // cd with no arg
        status = chdir(getenv("HOME"));
    }else if((stat(p_cmd->argv[1], &buff) == 0 && (S_IFDIR & buff.st_mode))){
        // cd with one arg
        status = chdir(p_cmd->argv[1]);
    }

    return status;
}

void cleanup(command_t *p_cmd){
    int i = 0;

    while(p_cmd->argv[i] != NULL){
        free(p_cmd->argv[i]);
        i++;
    }

    free(p_cmd->argv);
    free(p_cmd->path);	
}
