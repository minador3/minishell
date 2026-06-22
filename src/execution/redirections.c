/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:29:51 by mwei              #+#    #+#             */
/*   Updated: 2026/06/22 11:07:40 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_redirections(t_command *cmd)
{
    int fd;

    // 1. Handle Heredoc (<<)
    if (cmd->heredoc != NULL)
    {
        fd = process_heredoc(cmd->heredoc);
        if (fd != -1)
        {
            dup2(fd, STDIN_FILENO); // Reroute standard input to our heredoc pipe
            close(fd);
        }
    }
    // 2. Handle Infile (<) - Note: Only happens if there is NO heredoc!
    else if (cmd->infile != NULL)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd == -1)
        {
            perror("minishell: input file");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // 3. Handle Outfile / Append (>, >>)
    if (cmd->outfile != NULL)
    {
        if (cmd->append == 1)
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
        if (fd == -1)
        {
            perror("minishell: output file");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO); // Reroute standard output to the file
        close(fd);
    }
}
