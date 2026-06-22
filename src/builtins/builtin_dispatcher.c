/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:45:38 by mwei              #+#    #+#             */
/*   Updated: 2026/06/22 15:20:18 by mwei             ###   ########.fr       */
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
    if (ft_strncmp(cmd_name, "export", 7) == 0) return (1);
    if (ft_strncmp(cmd_name, "exit", 5) == 0) return (1);
    if (ft_strncmp(cmd_name, "unset", 6) == 0) return (1);
    return (0);
}

// Execute a builtin and return its exit status (0 on success)
int execute_builtin(t_command *cmd, char **envp, t_env **env_list)
{
    if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
        return (ft_pwd());
    else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
        return (ft_cd(cmd, env_list));
    else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
        return (ft_echo(cmd));
    else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
        return (ft_env(envp));
    else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
        return (ft_export(cmd, env_list)); // Pass the linked list here!
    else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
    {
        ft_exit(cmd);
        return (0);
    }
    else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
        return (ft_unset(cmd, env_list)); // Route to your new unset function!
    return (0);
}
