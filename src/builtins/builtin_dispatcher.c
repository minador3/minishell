/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:45:38 by mwei              #+#    #+#             */
/*   Updated: 2026/06/10 16:48:52 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd_name)
{
    if (cmd_name == NULL) return (0);
    if (ft_strncmp(cmd_name, "pwd", 4) == 0) return (1);
    if (ft_strncmp(cmd_name, "cd", 3) == 0) return (1);
    if (ft_strncmp(cmd_name, "echo", 5) == 0) return (1);
    if (ft_strncmp(cmd_name, "env", 4) == 0) return (1);
    if (ft_strncmp(cmd_name, "export", 7) == 0) return (1); // Added export
    if (ft_strncmp(cmd_name, "exit", 5) == 0) return (1);   // Added exit
    return (0);
}

void execute_builtin(t_command *cmd, char **envp)
{
    if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
        ft_pwd();
    else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
        ft_cd(cmd);
    else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
        ft_echo(cmd);
    else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
        ft_env(envp);
    else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
        ft_export(cmd, envp); // Added export
    else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
        ft_exit(cmd);         // Added exit
}
