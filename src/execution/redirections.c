/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:29:51 by mwei              #+#    #+#             */
/*   Updated: 2026/06/09 15:29:56 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h> // REQUIRED for O_RDONLY, O_CREAT, etc.

void handle_redirections(t_command *cmd)
{
    int fd;

    // 1. Handle Input Redirection (<)
    if (cmd->infile != NULL)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd == -1)
        {
            perror(cmd->infile);
            exit(1); // Standard bash behavior: exit child if input file is invalid
        }
        // Force STDIN (0) to read from our file
        dup2(fd, STDIN_FILENO); 
        close(fd); // Close the original fd since dup2 made a copy
    }

    // 2. Handle Output Redirection (> and >>)
    if (cmd->outfile != NULL)
    {
        if (cmd->append == 1) // For '>>'
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else // For '>'
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
        if (fd == -1)
        {
            perror(cmd->outfile);
            exit(1);
        }
        // Force STDOUT (1) to write to our file
        dup2(fd, STDOUT_FILENO); 
        close(fd);
    }
}
