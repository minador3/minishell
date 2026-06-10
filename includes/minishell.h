#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h> // Required for file operations like open()
# include "../libft/libft.h"

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

// --- Execution Prototypes ---
void execute_pipeline(t_command *cmd_list, char **envp);
char *get_path(char *cmd, char **envp);
void handle_redirections(t_command *cmd);

#endif