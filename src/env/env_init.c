/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:34 by mwei              #+#    #+#             */
/*   Updated: 2026/06/29 14:48:54 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

void	env_add_back(t_env **env_list, t_env *new_node)
{
	t_env	*temp;

	if (!env_list || !new_node)
		return ;
	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	temp = *env_list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_node;
}

static void	parse_and_add_env(t_env **env, char *str)
{
	char	*key;
	char	*val;
	char	*eq;

	eq = ft_strchr(str, '=');
	if (eq)
	{
		key = ft_substr(str, 0, eq - str);
		val = ft_substr(eq + 1, 0, ft_strlen(eq + 1));
	}
	else
	{
		key = ft_strdup(str);
		val = NULL;
	}
	env_add_back(env, new_env_node(key, val));
}

void	update_shlvl(t_env **env_list)
{
	t_env	*temp;
	int		level;
	char	*new_level_str;

	temp = *env_list;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->key, "SHLVL", 6) == 0)
		{
			level = ft_atoi(temp->value) + 1;
			new_level_str = ft_itoa(level);
			free(temp->value);
			temp->value = new_level_str;
			return ;
		}
		temp = temp->next;
	}
	env_add_back(env_list, new_env_node(ft_strdup("SHLVL"), ft_strdup("1")));
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		parse_and_add_env(&env, envp[i++]);
	update_shlvl(&env);
	return (env);
}
