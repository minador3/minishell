#include "minishell.h"

int	is_redirection(int type)
{
	if (type == REDIR_IN
		|| type == REDIR_OUT
		|| type == APPEND)
		return (1);
	return (0);
}

char	*token_to_str(int type)
{
	if (type == PIPE)
		return ("|");
	if (type == REDIR_IN)
		return ("<");
	if (type == REDIR_OUT)
		return (">");
	if (type == APPEND)
		return (">>");
	if (type == HEREDOC)
		return ("<<");
	return ("unknown");
}

int	count_cmd_args(t_token *token)
{
	int		i;
	t_token	*head;

	head = token;
	i = 0;
	while (head != NULL && head->type != PIPE)
	{
		if (head->type == WORD)
		{
			i++;
			head = head->next;
		}
		else if (head->type == REDIR_IN)
			head = head->next->next;
		else if (head->type == REDIR_OUT)
			head = head->next->next;
		else if (head->type == HEREDOC)
			head = head->next;
		else if (head->type == APPEND)
			head = head->next->next;
	}
	return (i);
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->append = 0;
	cmd->argv = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	allocate_cmd_arg(t_cmd *cmd, t_token *token)
{
	int	count;

	count = count_cmd_args(token);
	cmd->argv = malloc(sizeof(char *) * (count + 1));
	if (!cmd->argv)
		return (1);
	return (0);
}
