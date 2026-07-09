/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:37 by mwei              #+#    #+#             */
/*   Updated: 2026/06/29 14:48:21 by mwei             ###   ########.fr       */
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

static int	update_existing(t_env *env, const char *key, const char *val)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
		{
			free(env->value);
			env->value = NULL;
			if (val)
				env->value = ft_strdup(val);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int	env_set_value(t_env **env, const char *key, const char *val)
{
	char	*nk;
	char	*nv;

	if (!key)
		return (-1);
	if (update_existing(*env, key, val))
		return (0);
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
	char	*join;

	envp = malloc(sizeof(char *) * (count_env_nodes(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value != NULL && ft_strncmp(env->key, "?", 2) != 0)
		{
			join = ft_strjoin(env->key, "=");
			envp[i++] = ft_strjoin(join, env->value);
			free(join);
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
