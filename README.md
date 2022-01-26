# Final Project for COMP 211 - A Simple Shell
A simple shell made in C that executes simple system programs. It can execute special built-in commands like `exit` and `cd`, and common
commands such as `pwd`, `ls`, and `mkdir`.

### Important Note
This shell was designed to be used within a UNIX operating system, as it provided consistency
within the students' code. Specifically, this shell was made within a Dockerized Ubuntu environment. The shell WILL NOT work on a different processor architecture than x86_64.
Please see `shell demonstration.png` to see how the shell works in action.

### Compiling and Running
Compile and run with `make` and `make run`. Use `exit` to exit.

## Project Structure
There are 4 main files that this project is comprised of:
* `Makefile` - contains overlying information needed to compile the shell with `make`
* `shell.c` - contains all function definitions. The main functions at work here are `parse`, `find_fullpath`, and `execute`.
* `shell.h` - includes declarations and specifications for the functions contained in `shell.c`.
* `driver.c` - includes the main function, a loop that reads in a typed command and uses the `shell.c` functions to determine if the command exists, and how to handle it. 

## How This Shell Works
1. The shell takes in input, and parses it through the `parse` function. The `parse` function requires a `command_t` struct as one of its parameter types. The struct encompasses the description of a command, including a path, the total number of arguments given, and what those arguments are. If `path` needs to be an absolute path, `find_fullpath` will be used to do so.
```c
void parse(char *line, command_t *p_cmd);
```
```c
typedef struct {
    char *path;
    int argc;
    char **argv;
} command_t;
```

2. A loop is defined so that each folder defined in the `PATH` environment variable is checked for the command's binaries. This is done through the `find_fullpath` function. The `stat` function, used within `find_fullpath`, verifies if the file/directory exists in the filesystem. If it does, `find_fullpath` returns true and the `path` variable in the `command_t` struct is set to the absolute path of the binary.

3. The `execute` function is then called to execute the given command using the fork-exec-wait paradigm. The process forks itself; the parent waits for the child to terminate, while the child spawns its own unique process through `execv()`. If the process forks successfully, the `status` variable will never be set to `ERROR` and the `execute` function will return `SUCCESSFUL`.