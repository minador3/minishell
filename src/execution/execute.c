/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:26:53 by mwei              #+#    #+#             */
/*   Updated: 2026/06/17 16:56:47 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_pipeline(t_command *cmd_list, char **envp, t_env **env_list)
{
    int     fd[2];
    int     prev_read_fd = -1;
    pid_t   pid;
    int     status;
    char    *path;

    while (cmd_list != NULL)
    {
        // --- 1. PARENT BUILT-IN INTERCEPTOR ---
        // If there are no pipes (it's a single command) and it's a built-in, execute it in the parent!
        // We do this so commands like 'cd' actually change the parent shell's directory.
        if (cmd_list->next == NULL && prev_read_fd == -1 && cmd_list->args && is_builtin(cmd_list->args[0]))
        {
            // Note: In the future, you will want to temporarily apply redirections here too
            execute_builtin(cmd_list, envp, env_list);
            cmd_list = cmd_list->next;
            continue; // Skip the fork completely!
        }

        // --- 2. PIPES ---
        // Create a pipe if there is a next command in the list
        if (cmd_list->next != NULL)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe error");
                return;
            }
        }

        // --- 3. FORK ---
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

            // D. Safety check: Did the user just type "> file.txt" with no command?
            if (cmd_list->args == NULL || cmd_list->args[0] == NULL)
                exit(0);

            // E. Child Built-in Interceptor
            // If it's part of a pipe (e.g., "pwd | grep /"), it runs in the child
            if (is_builtin(cmd_list->args[0]))
            {
                execute_builtin(cmd_list, envp, env_list);
                exit(0); // Built-in finished, kill the child process cleanly
            }

            // F. Execute External Commands (ls, grep, cat, etc.)
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

    // --- 4. WAIT ---
    // Wait for all child processes to finish before returning to the prompt
    while (waitpid(-1, &status, 0) > 0)
        ;
}
