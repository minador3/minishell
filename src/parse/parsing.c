#include "minishell.h"

int	handle_pipe_case(t_cmd **current, t_token **head, int *i, t_cmd *head_cmd)
{
	(*current)->argv[*i] = NULL;
	(*current)->next = init_cmd();
	if (!((*current)->next))
	{
		free_cmd(head_cmd);
		return (1);
	}
	(*current) = (*current)->next;
	(*head) = (*head)->next;
	if (allocate_cmd_arg(*current, *head) == 1)
	{
		free_cmd(head_cmd);
		return (1);
	}
	*i = 0;
	return (0);
}

void	handle_redir(t_cmd *current, t_token **head)
{
	if ((*head)->type == REDIR_IN)
	{
		current->input_file = ft_strdup((*head)->next->value);
		(*head) = (*head)->next->next;
	}
	else if ((*head)->type == REDIR_OUT)
	{
		current->output_file = ft_strdup((*head)->next->value);
		(*head) = (*head)->next->next;
	}
	else if ((*head)->type == APPEND)
	{
		current->output_file = ft_strdup((*head)->next->value);
		current->append = 1;
		(*head) = (*head)->next->next;
	}
	else if ((*head)->type == HEREDOC)
	{
		if ((*head)->value)
			current->heredoc = ft_strdup((*head)->value);
		(*head) = (*head)->next;
	}
}

int	handle_token(t_cmd **current, t_token **head, int *i, t_cmd *head_cmd)
{
	if ((*head)->type == WORD)
	{
		(*current)->argv[(*i)++] = ft_strdup((*head)->value);
		(*head) = (*head)->next;
	}
	else if (is_redirection((*head)->type) || (*head)->type == HEREDOC)
		handle_redir(*current, head);
	else if ((*head)->type == PIPE)
	{
		if (handle_pipe_case(current, head, i, head_cmd))
			return (1);
	}
	return (0);
}

t_cmd	*parse_init(t_token *token, t_cmd **current)
{
	int		count;
	t_cmd	*head_cmd;

	count = count_cmd_args(token);
	head_cmd = init_cmd();
	if (!head_cmd)
		return (NULL);
	*current = head_cmd;
	(*current)->argv = malloc(sizeof(char *) * (count + 1));
	if (!(*current)->argv)
	{
		free_cmd(head_cmd);
		return (NULL);
	}
	return (head_cmd);
}

t_cmd	*parse(t_token *token)
{
	t_token	*head;
	t_cmd	*head_cmd;
	t_cmd	*current_cmd;
	int		i;

	head_cmd = parse_init(token, &current_cmd);
	if (!head_cmd)
		return (NULL);
	head = token;
	i = 0;
	while (head != NULL)
	{
		if (handle_token(&current_cmd, &head, &i, head_cmd))
			return (NULL);
	}
	current_cmd->argv[i] = NULL;
	return (head_cmd);
}
