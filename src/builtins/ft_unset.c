/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:24 by mwei              #+#    #+#             */
/*   Updated: 2026/07/08 14:39:21 by mwei             ###   ########.fr       */
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

static int	is_valid_id(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	unset_var(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
		{
			remove_node(env, tmp, prev);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_cmd *cmd, t_env **env)
{
	int		i;
	int		status;

	i = 1;
	status = 0;
	while (cmd->argv[i])
	{
		if (!is_valid_id(cmd->argv[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n",
				cmd->argv[i]);
			status = 1;
		}
		else if (ft_strncmp(cmd->argv[i], "?", 2) != 0)
		{
			unset_var(env, cmd->argv[i]);
		}
		i++;
	}
	return (status);
}
