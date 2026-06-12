/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:27 by mwei              #+#    #+#             */
/*   Updated: 2026/06/10 16:46:51 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_export(t_command *cmd, char **envp)
{
    int i = 0;

    // 1. If no arguments, print the environment with "declare -x"
    if (cmd->args[1] == NULL)
    {
        if (!envp)
            return;
        while (envp[i] != NULL)
        {
            // Note: A true bash export also sorts these alphabetically! 
            printf("declare -x %s\n", envp[i]);
            i++;
        }
        return;
    }

    // 2. If there are arguments, we need to add/update them.
    // WARNING: This requires you to have duplicated 'envp' with malloc first!
    i = 1;
    while (cmd->args[i] != NULL)
    {
        // Placeholder for future logic:
        // - Check if argument has an '=' sign
        // - If it exists in envp, replace the string
        // - If it doesn't exist, realloc envp array to be +1 larger and add it
        printf("minishell: export: %s: not fully implemented yet\n", cmd->args[i]);
        i++;
    }
}