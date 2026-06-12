/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:19 by mwei              #+#    #+#             */
/*   Updated: 2026/06/10 16:16:06 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(t_command *cmd)
{
    int i = 1;
    int n_flag = 0;

    if (cmd->args == NULL || cmd->args[1] == NULL)
    {
        printf("\n");
        return;
    }

    // Check for the -n flag(s)
    // We loop here to handle edge cases like: echo -n -n -n hello
    while (cmd->args[i] != NULL && ft_strncmp(cmd->args[i], "-n", 3) == 0)
    {
        n_flag = 1;
        i++;
    }

    // Print the remaining arguments
    while (cmd->args[i] != NULL)
    {
        printf("%s", cmd->args[i]);
        
        // If there is another argument after this one, print a space
        if (cmd->args[i + 1] != NULL)
            printf(" ");
            
        i++;
    }

    // Print a newline ONLY if the -n flag was not triggered
    if (n_flag == 0)
        printf("\n");
}
