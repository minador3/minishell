/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:26:53 by mwei              #+#    #+#             */
/*   Updated: 2026/05/26 15:27:51 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// For now, we will hardcode the path to "/bin/ls". 
// Later, you will write a function to search the "PATH" variable.
void execute_pipeline(t_command *cmd_list, char **envp)
{
    pid_t pid;
    int status;

    if (cmd_list == NULL)
        return;

    // 1. Create a child process
    pid = fork();
    
    if (pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    
    if (pid == 0)
    {
        // --- WE ARE IN THE CHILD PROCESS ---
        
        // Let's hardcode the absolute path just to see it work
        char *path = "/bin/ls"; 
        
        // execve replaces the child process with the 'ls' program
        if (execve(path, cmd_list->args, envp) == -1)
        {
            perror("execve failed");
            exit(127);
        }
    }
    else
    {
        // --- WE ARE IN THE PARENT PROCESS (Minishell) ---
        
        // Wait for the child (the 'ls' command) to finish before continuing
        waitpid(pid, &status, 0);
    }
}