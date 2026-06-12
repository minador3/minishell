/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:22 by mwei              #+#    #+#             */
/*   Updated: 2026/06/10 16:34:12 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_env(char **envp)
{
    int i;

    if (envp == NULL)
        return;
    i = 0;
    while (envp[i] != NULL)
    {
        // Standard bash 'env' only prints variables that have an '=' sign
        if (ft_strchr(envp[i], '=') != NULL)
        {
            printf("%s\n", envp[i]);
        }
        i++;
    }
}