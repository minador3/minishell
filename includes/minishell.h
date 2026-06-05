#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>

// Your core data structure
typedef struct s_command
{
    char                **args;     // e.g., {"ls", "-l", NULL}
    char                *infile;    // e.g., "input.txt" or NULL
    char                *outfile;   // e.g., "output.txt" or NULL
    int                 append;     // 1 for '>>', 0 for '>'
    char                *heredoc;   // string for '<<' or NULL
    struct s_command    *next;      // Next command in the pipeline
} t_command;

#endif