/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:37 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 17:30:46 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_get_value(t_env *env_list, const char *key)
{
	if (!key)
		return (NULL);
	while (env_list)
	{
		if (ft_strncmp(env_list->key, key, ft_strlen(key) + 1) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

int	env_set_value(t_env **env, const char *key, const char *val)
{
	t_env	*n;
	char	*nk;
	char	*nv;

	if (!key)
		return (-1);
	n = *env;
	while (n)
	{
		if (ft_strncmp(n->key, key, ft_strlen(key) + 1) == 0)
		{
			free(n->value);
			n->value = NULL;
			if (val)
				n->value = ft_strdup(val);
			return (0);
		}
		n = n->next;
	}
	nk = ft_strdup(key);
	nv = NULL;
	if (val)
		nv = ft_strdup(val);
	if (!nk || (val && !nv))
		return (free(nk), -1);
	env_add_back(env, new_env_node(nk, nv));
	return (0);
}

static int	count_env_nodes(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	**env_list_to_envp(t_env *env)
{
	int		i;
	char	**envp;
	char	*joined;

	envp = malloc(sizeof(char *) * (count_env_nodes(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			joined = ft_strjoin(env->key, "=");
			envp[i++] = ft_strjoin(joined, env->value);
			free(joined);
		}
		else
			envp[i++] = ft_strdup(env->key);
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
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
