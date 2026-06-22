/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 14:29:55 by mwei              #+#    #+#             */
/*   Updated: 2026/06/22 13:49:24 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_cd(t_command *cmd, t_env **env_list)
{
    char *path;
    char *cwd_before;
    char *newcwd;

    // too many arguments check
    if (cmd->args[1] != NULL && cmd->args[2] != NULL)
    {
        printf("minishell: cd: too many arguments\n");
        return (1);
    }

    // Save current working directory for OLDPWD
    cwd_before = getcwd(NULL, 0);

    // No argument: use HOME
    if (cmd->args[1] == NULL)
    {
        path = env_get_value(*env_list, "HOME");
        if (path == NULL)
        {
            printf("minishell: cd: HOME not set\n");
            free(cwd_before);
            return (1);
        }
    }
    else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
    {
        // cd - : go to OLDPWD and print it
        path = env_get_value(*env_list, "OLDPWD");
        if (path == NULL)
        {
            printf("minishell: cd: OLDPWD not set\n");
            free(cwd_before);
            return (1);
        }
        printf("%s\n", path);
    }
    else
    {
        path = cmd->args[1];
    }

    if (chdir(path) != 0)
    {
        perror("minishell: cd");
        free(cwd_before);
        return (1);
    }

    // Update OLDPWD to previous cwd
    if (cwd_before != NULL)
        env_set_value(env_list, "OLDPWD", cwd_before);

    // Update PWD to new cwd
    newcwd = getcwd(NULL, 0);
    if (newcwd != NULL)
    {
        env_set_value(env_list, "PWD", newcwd);
        free(newcwd);
    }
    free(cwd_before);
    return (0);
}
