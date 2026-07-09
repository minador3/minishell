/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:40 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 21:38:10 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_sig_check(void)
{
	if (g_sig == SIGINT)
		rl_done = 1;
	return (0);
}

static void	write_line_to_pipe(int fd, char *line, t_env *env_list)
{
	char	*expanded;

	expanded = expand_heredoc_line(line, env_list);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

static int	heredoc_init(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("minishell: heredoc pipe");
		return (-1);
	}
	return (0);
}

int	process_heredoc(char *delimiter, t_env *env_list)
{
	int		fd[2];
	char	*line;

	if (heredoc_init(fd) == -1)
		return (-1);
	rl_event_hook = &heredoc_sig_check;
	while (1)
	{
		g_sig = 0;
		line = readline("> ");
		if (!line || g_sig == SIGINT
			|| ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		write_line_to_pipe(fd[1], line, env_list);
	}
	rl_event_hook = NULL;
	free(line);
	close(fd[1]);
	if (g_sig == SIGINT)
	{
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}

int	preprocess_heredocs(t_cmd *cmd_list, t_env **env_list)
{
	t_cmd	*curr;

	curr = cmd_list;
	while (curr != NULL)
	{
		if (curr->heredoc != NULL)
		{
			curr->heredoc_fd = process_heredoc(curr->heredoc, *env_list);
			if (curr->heredoc_fd == -1)
			{
				update_exit_status(env_list, 130);
				return (-1);
			}
		}
		curr = curr->next;
	}
	return (0);
}
