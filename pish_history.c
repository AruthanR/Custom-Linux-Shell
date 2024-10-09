#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
//used chatGPT to debug errors in these methods
#include "pish.h"

static char pish_history_path[1024] = {'\0'};

/*
 * Set history file path to ~/.pish_history.
 */
static void set_history_path()
{
    const char *home = getpwuid(getuid())->pw_dir;
    strncpy(pish_history_path, home, 1024);
    strcat(pish_history_path, "/.pish_history");
}

void add_history(const struct pish_arg *arg)
{
    // set history path if needed
    if (!(*pish_history_path)) {
        set_history_path();
    }

    /* 
     * TODO:
     * - open (and create if needed) history file at pish_history_path.
     * - write out the command stored in `arg`; argv values are separated
     *   by a space.
     */
    FILE *history_file = fopen(pish_history_path, "a");
    if (history_file == NULL) {
        perror("open");
        return;
    }
    for (int i = 0; i < arg->argc; i++) {
        fprintf(history_file, "%s", arg->argv[i]);
        if (i < arg->argc - 1) {
            fprintf(history_file, " ");
        }
    }
    fprintf(history_file, "\n");
    fclose(history_file);
}

void print_history()
{
    // set history path if needed
    if (!(*pish_history_path)) {
        set_history_path();
    }

    /* TODO: read history file and print with index */
    FILE *history_file = fopen(pish_history_path, "r");
    if (history_file == NULL) {
        perror("open");
        return;
    }

    char line[1024];
    int counter = 1;
    while (fgets(line, sizeof(line), history_file)) {
        printf("%d %s", counter++, line);
    }

    fclose(history_file);
}
