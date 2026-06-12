/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 14:29:55 by mwei              #+#    #+#             */
/*   Updated: 2026/06/10 15:27:14 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_command *cmd)
{
    // If the user just types "cd" with no arguments
    if (cmd->args[1] == NULL)
    {
        // In a full implementation, you will need to look up "HOME" in envp.
        // For now, let's just print a placeholder message.
        printf("minishell: cd: HOME not set (to be implemented!)\n");
        return;
    }

    // Try to change the directory
    if (chdir(cmd->args[1]) != 0)
    {
        // If it fails (e.g., folder doesn't exist), print the standard error
        perror("minishell: cd");
    }
}
