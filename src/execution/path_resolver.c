/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:18:58 by mwei              #+#    #+#             */
/*   Updated: 2026/06/09 15:19:06 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to free a 2D array (like the one returned by ft_split)
void free_split(char **split_arr)
{
    int i = 0;
    if (!split_arr)
        return;
    while (split_arr[i])
    {
        free(split_arr[i]);
        i++;
    }
    free(split_arr);
}

char *get_path(char *cmd, char **envp)
{
    int     i;
    char    **paths;
    char    *dir_slash;
    char    *full_path;

    // 1. Edge Case: If the command is already a valid absolute or relative path
    if (access(cmd, X_OK) == 0)
        return (ft_strdup(cmd)); // Returns a copy of the string

    // 2. Find the "PATH=" variable in envp
    i = 0;
    while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    
    if (envp[i] == NULL) // PATH variable doesn't exist
        return (NULL);

    // 3. Split the directories (envp[i] + 5 skips the "PATH=" part)
    paths = ft_split(envp[i] + 5, ':'); //
    if (!paths)
        return (NULL);

    // 4. Test each directory
    i = 0;
    while (paths[i])
    {
        dir_slash = ft_strjoin(paths[i], "/"); // e.g., "/bin" + "/" -> "/bin/"
        full_path = ft_strjoin(dir_slash, cmd); // e.g., "/bin/" + "ls" -> "/bin/ls"
        free(dir_slash); // Clean up the intermediate string

        if (access(full_path, X_OK) == 0)
        {
            free_split(paths); // Clean up our split array before returning
            return (full_path); // We found the executable!
        }
        free(full_path); // It wasn't here, free it and try the next one
        i++;
    }
    
    free_split(paths);
    return (NULL); // Command not found in any PATH directory
}