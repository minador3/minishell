/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:40 by mwei              #+#    #+#             */
/*   Updated: 2026/06/22 11:06:54 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

// Captures input until the delimiter is found, returns a file descriptor containing the text
int process_heredoc(char *delimiter)
{
    int     fd[2];
    char    *line;

    // Create a pipe to hold the user's input in memory
    if (pipe(fd) == -1)
    {
        perror("minishell: heredoc pipe");
        return (-1);
    }

    while (1)
    {
        // Readline automatically handles the standard bash "> " prompt for heredocs
        line = readline("> ");
        
        // If ctrl-D is pressed (EOF), line is NULL, so we break
        if (!line)
            break;

        // If the user types the exact delimiter, we stop reading
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
        {
            free(line);
            break;
        }

        // Write the line into our pipe, followed by a newline character
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }

    // Crucial: Close the write end of the pipe, or the command reading from it will hang forever!
    close(fd[1]);

    // Return the read end so the command can use it as input
    return (fd[0]);
}
