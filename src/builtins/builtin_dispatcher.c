/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:45:38 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 17:16:40 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	if (ft_strncmp(cmd_name, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd_name, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd_name, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd_name, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd_name, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd_name, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd_name, "exit", 5) == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_cmd *cmd, char **envp, t_env **env_list)
{
	(void)envp;
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (ft_echo(cmd));
	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		return (ft_cd(cmd, env_list));
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		return (ft_export(cmd, env_list));
	if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		return (ft_unset(cmd, env_list));
	if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		return (ft_env(env_list));
	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
	{
		ft_exit(cmd);
		return (0);
	}
	return (127);
}
