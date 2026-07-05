/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:09:26 by mwei              #+#    #+#             */
/*   Updated: 2026/06/29 14:02:50 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_id(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->key, "?", 2) && ft_strncmp(env->key, "_", 2))
		{
			if (env->value)
				printf("declare -x %s=\"%s\"\n", env->key, env->value);
			else
				printf("declare -x %s\n", env->key);
		}
		env = env->next;
	}
}

static void	update_node(t_env **env, char *k, char *v)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, k, ft_strlen(k) + 1) == 0)
		{
			if (v)
			{
				free(tmp->value);
				tmp->value = ft_strdup(v);
			}
			free(k);
			if (v)
				free(v);
			return ;
		}
		tmp = tmp->next;
	}
	env_add_back(env, new_env_node(k, v));
}

static void	handle_arg(t_env **env, char *arg)
{
	char	*eq;
	char	*k;
	char	*v;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		k = ft_substr(arg, 0, eq - arg);
		v = ft_substr(eq + 1, 0, ft_strlen(eq + 1));
	}
	else
	{
		k = ft_strdup(arg);
		v = NULL;
	}
	update_node(env, k, v);
}

int	ft_export(t_cmd *cmd, t_env **env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!cmd->argv[1])
		print_export(*env);
	else
	{
		while (cmd->argv[i])
		{
			if (!is_valid_id(cmd->argv[i]))
			{
				printf("minishell: export: `%s': not a valid id\n",
					cmd->argv[i]);
				status = 1;
			}
			else
				handle_arg(env, cmd->argv[i]);
			i++;
		}
	}
	return (status);
}
