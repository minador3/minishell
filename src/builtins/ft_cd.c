/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 14:29:55 by mwei              #+#    #+#             */
/*   Updated: 2026/07/07 19:07:08 by mwei             ###   ########.fr       */
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

int	ft_cd(t_cmd *cmd, t_env **env)
{
	char	*path;
	char	old_cwd[1024];
	char	new_cwd[1024];

	old_cwd[0] = '\0';
	getcwd(old_cwd, 1024);
	if (cmd->argv[1] == NULL)
		path = get_env_val(*env, "HOME");
	else
		path = cmd->argv[1];
	if (!path)
	{
		printf("minishell: cd: HOME not set\n");
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (old_cwd[0] != '\0')
		env_set_value(env, "OLDPWD", old_cwd);
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		env_set_value(env, "PWD", new_cwd);
	return (0);
}
