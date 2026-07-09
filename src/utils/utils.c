#include "minishell.h"

t_token	*ft_lexernew(int type, char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (str)
		token->value = ft_strdup(str);
	else
		token->value = NULL;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	ft_lexeradd_back(t_token **head, t_token *new)
{
	t_token	*temp;

	temp = *head;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	ft_listclear(t_token **lst)
{
	t_token	*n;

	if (*lst == NULL)
		return ;
	while (*lst)
	{
		n = (*lst)->next;
		ft_listdelone(*lst);
		*lst = n;
	}
	*lst = NULL;
}

void	ft_listdelone(t_token *lst)
{
	if (lst == NULL)
		return ;
	free(lst->value);
	free(lst);
}

void	free_cmd(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;

	if (!cmd)
		return ;
	tmp = NULL;
	while (cmd)
	{
		i = 0;
		if (cmd->argv)
		{
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		free(cmd->input_file);
		free(cmd->output_file);
		free(cmd->heredoc);
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
}

char	*realloc_word_buffer(char *old, int k, int new_size)
{
	char	*new_str;
	int		i;

	new_str = malloc(new_size);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < k)
	{
		new_str[i] = old[i];
		i++;
	}
	new_str[i] = '\0';
	free(old);
	return (new_str);
}
