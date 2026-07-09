/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:26:53 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 19:37:39 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_token	*temp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		free((*lst)->value);
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
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
		if (cmd->heredoc_fd != -1)
			close(cmd->heredoc_fd);
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
}

void	free_envp_array(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
