/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:26:53 by mwei              #+#    #+#             */
/*   Updated: 2026/06/09 16:09:18 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_pipeline(t_command *cmd_list, char **envp)
{
    int     fd[2];
    int     prev_read_fd = -1;
    pid_t   pid;
    int     status;
    char    *path;

    while (cmd_list != NULL)
    {
        // 1. Create a pipe if there is a next command
        if (cmd_list->next != NULL)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe error");
                return;
            }
        }

        // 2. Fork the process
        pid = fork();
        if (pid == -1)
        {
            perror("fork error");
            return;
        }

        if (pid == 0) // --- CHILD PROCESS ---
        {
            // A. Receive input from the PREVIOUS command's pipe
            if (prev_read_fd != -1)
            {
                dup2(prev_read_fd, STDIN_FILENO);
                close(prev_read_fd);
            }

            // B. Send output to the NEXT command's pipe
            if (cmd_list->next != NULL)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                close(fd[0]); // Child doesn't read from its own output pipe
            }

            // C. File Redirections (These happen AFTER pipes, so ">" overrides "|")
            handle_redirections(cmd_list);

            // D. Execute the command
            if (cmd_list->args == NULL || cmd_list->args[0] == NULL)
                exit(0);

            path = get_path(cmd_list->args[0], envp);
            if (!path)
            {
                printf("minishell: %s: command not found\n", cmd_list->args[0]);
                exit(127);
            }

            if (execve(path, cmd_list->args, envp) == -1)
            {
                perror("execve failed");
                free(path);
                exit(126);
            }
        }
        else // --- PARENT PROCESS ---
        {
            // A. Close the previous read_fd because we are done with it
            if (prev_read_fd != -1)
                close(prev_read_fd);

            // B. Save the current pipe's read end for the NEXT command in the loop
            if (cmd_list->next != NULL)
            {
                prev_read_fd = fd[0];
                close(fd[1]); // The parent never writes to the pipe!
            }
        }

        // Move to the next command in the linked list
        cmd_list = cmd_list->next;
    }

    // 3. Wait for all child processes to finish before returning to the prompt
    while (waitpid(-1, &status, 0) > 0)
        ;
}