/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:24 by mwei              #+#    #+#             */
/*   Updated: 2026/06/22 13:46:56 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to check if a string is a valid number
static int is_numeric(char *str)
{
    int i = 0;
    
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (str[i] == '\0') // Catch lone '+' or '-'
        return (0);
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9') // Basically ft_isdigit
            return (0);
        i++;
    }
    return (1);
}

void ft_exit(t_command *cmd)
{
    printf("exit\n"); // Bash always prints "exit" when the command is run

    // 1. No arguments: just exit
    if (cmd->args[1] == NULL)
        exit(0);

    // 2. Non-numeric argument: print error and exit 255
    if (!is_numeric(cmd->args[1]))
    {
        printf("minishell: exit: %s: numeric argument required\n", cmd->args[1]);
        exit(255);
    }

    // 3. Too many arguments: print error, but DO NOT exit
    if (cmd->args[2] != NULL)
    {
        printf("minishell: exit: too many arguments\n");
        return; 
    }

    // 4. Valid numeric argument: exit with that number (modulo 256 to simulate 8-bit wrap around)
    exit(ft_atoi(cmd->args[1]) % 256);
}
