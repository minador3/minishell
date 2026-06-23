/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:09:46 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 17:17:12 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_node(t_env **env, t_env *node, t_env *prev)
{
	if (prev == NULL)
		*env = node->next;
	else
		prev->next = node->next;
	free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

int	ft_unset(t_command *cmd, t_env **env)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	i = 1;
	while (cmd->args[i])
	{
		tmp = *env;
		prev = NULL;
		while (tmp)
		{
			if (ft_strncmp(tmp->key, cmd->args[i], ft_strlen(cmd->args[i])
					+ 1) == 0)
			{
				remove_node(env, tmp, prev);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (0);
}
