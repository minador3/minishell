/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:24:06 by mwei              #+#    #+#             */
/*   Updated: 2026/06/10 15:24:08 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_pwd(void)
{
    char *cwd;

    // Passing NULL and 0 tells getcwd to malloc the perfect size
    cwd = getcwd(NULL, 0); 
    
    if (cwd != NULL)
    {
        printf("%s\n", cwd);
        free(cwd); // You MUST free it since getcwd malloc'd it!
    }
    else
    {
        perror("minishell: pwd");
    }
}