/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:40 by mwei              #+#    #+#             */
/*   Updated: 2026/07/07 03:48:04 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper to isolate the error printing and save lines
static int	heredoc_error(void)
{
	perror("minishell: heredoc pipe");
	return (-1);
}

// Helper to prevent file descriptor leaks if ctrl-C is pressed!
static int	clean_exit(int *fd, char *line, int ret)
{
	if (line)
		free(line);
	close(fd[0]);
	close(fd[1]);
	return (ret);
}

int	process_heredoc(char *delimiter, t_env *env_list)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (heredoc_error());
	while (1)
	{
		g_sig = 0;
		line = readline("> ");
		if (g_sig == SIGINT)
			return (clean_exit(fd, line, -1));
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	if (line)
		free(line);
	close(fd[1]);
	return (fd[0]);
}
