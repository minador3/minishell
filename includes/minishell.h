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
# include <errno.h>

typedef struct s_env
{
	char *key;   // e.g., "USER"
	char *value; // e.g., "mwei"
	struct s_env	*next;
}					t_env;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**argv;
	char			*input_file;
	char			*output_file;
	int				append;
	char 			*heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_word
{
    char    *str;
    char    quote_state;
    int     k;
    t_env   *env_list;
}   t_word;

// --- Execution Prototypes ---
void				execute_pipeline(t_cmd *cmd_list, t_env **env_list);
char				*get_path(char *cmd, char **envp);
void				handle_redirections(t_cmd *cmd, t_env *env_list);
int					process_heredoc(char *delimiter, t_env *env_list);
char	*expand_heredoc_line(char *line, t_env *env);
void				update_exit_status(t_env **env_list, int status);
void				free_envp_array(char **envp);
void				wait_for_children(pid_t last_pid, t_env **env_list);

// --- Builtin Prototypes ---
int     is_builtin(char *cmd_name);
int     execute_builtin(t_cmd *cmd, char **envp, t_env **env_list);
int     ft_pwd(void);
int     ft_cd(t_cmd *cmd, t_env **env_list);
int     ft_echo(t_cmd *cmd);
int     ft_env(t_env **env_list);
void    ft_exit(t_cmd *cmd);
int     ft_export(t_cmd *cmd, t_env **env_list);
int     ft_unset(t_cmd *cmd, t_env **env_list);

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

// Lexer/token
int 	types(char c);
void    handle_operator(t_token **token, char *line, int *j);
void    handle_heredoc(t_token **token, char *line, int *j);
void    handle_expansion(t_word *w, char *line, int *j);
void    handle_word(t_token **token, char *line, int *j, t_env *env_list);
void    expand_exit_status(t_word *w, int *j);
void    expand_variable(t_word *w, char *line, int *j);
char    *extract_varname(char *line, int *j);
void    fill_word(t_word *w, char *line, int *j);


// parsing
int		is_redirection(int type);
char	*token_to_str(int type);
int		syntax_check(t_token *token);
int		check_tokens(t_token *head);
int		count_cmd_args(t_token *token);
t_cmd	*init_cmd(void);
int		allocate_cmd_arg(t_cmd *cmd, t_token *token);
t_cmd	*parse(t_token *token);


// utils
t_token *ft_lexernew(int type, char *str);
void    ft_lexeradd_back(t_token **head, t_token *new);
void	ft_listclear(t_token **lst);
void	ft_listdelone(t_token *lst);
void	free_cmd(t_cmd *cmd);

void	shell_loop(t_env **env_list);
t_token	*tokenizer(char *line, t_env *env_list);


#endif
