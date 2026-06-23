/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 14:29:55 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 17:16:46 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_val(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	ft_cd(t_command *cmd, t_env **env)
{
	char	*path;

	if (cmd->args[1] == NULL)
	{
		path = get_env_val(*env, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = cmd->args[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
