#ifndef MINISHELL_H
# define MINISHELL_H

# define _XOPEN_SOURCE 700
extern int g_sig;

# include "../libft/libft.h"
# include <fcntl.h> // Required for file operations like open()
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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
}					t_command;

typedef struct s_env
{
	char *key;   // e.g., "USER"
	char *value; // e.g., "mwei"
	struct s_env	*next;
}					t_env;

// --- Execution Prototypes ---
void				execute_pipeline(t_command *cmd_list, t_env **env_list);
char				*get_path(char *cmd, char **envp);
void				handle_redirections(t_command *cmd);
int					process_heredoc(char *delimiter);
void				update_exit_status(t_env **env_list, int status);
void				free_envp_array(char **envp);
void				wait_for_children(pid_t last_pid, t_env **env_list);

// --- Builtin Prototypes ---
int     is_builtin(char *cmd_name);
int     execute_builtin(t_command *cmd, char **envp, t_env **env_list);
int     ft_pwd(void);
int     ft_cd(t_command *cmd, t_env **env_list);
int     ft_echo(t_command *cmd);
int     ft_env(t_env **env_list);
void    ft_exit(t_command *cmd);
int     ft_export(t_command *cmd, t_env **env_list);
int     ft_unset(t_command *cmd, t_env **env_list);

// --- Env Prototypes ---
t_env				*init_env(char **envp);
t_env				*new_env_node(char *key, char *value);
void				env_add_back(t_env **env_list, t_env *new_node);
char				*env_get_value(t_env *env_list, const char *key);
int					env_set_value(t_env **env_list, const char *key,
						const char *value);
char				**env_list_to_envp(t_env *env_list);

void				update_shlvl(t_env **env_list);

void				setup_signals(void);

#endif
