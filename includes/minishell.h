#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h> // Required for file operations like open()
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

// Your core data structure
typedef struct s_command
{
	char **args;            // e.g., {"ls", "-l", NULL}
	char *infile;           // e.g., "input.txt" or NULL
	char *outfile;          // e.g., "output.txt" or NULL
	int append;             // 1 for '>>', 0 for '>'
	char *heredoc;          // string for '<<' or NULL
	struct s_command *next; // Next command in the pipeline
}		t_command;

// --- Execution Prototypes ---
void    execute_pipeline(t_command *cmd_list, char **envp);
char    *get_path(char *cmd, char **envp);
void    handle_redirections(t_command *cmd);

// --- Builtin Prototypes ---
int     is_builtin(char *cmd_name);
void    execute_builtin(t_command *cmd, char **envp);
void    ft_pwd(void);
void    ft_cd(t_command *cmd);
void    ft_echo(t_command *cmd);
void    ft_env(char **envp);
void    ft_exit(t_command *cmd);
void    ft_export(t_command *cmd, char **envp);

#endif