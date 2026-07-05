/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:29:51 by mwei              #+#    #+#             */
/*   Updated: 2026/06/29 15:09:50 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input(t_cmd *cmd)
{
	int	fd;

	if (cmd->heredoc != NULL)
	{
		fd = process_heredoc(cmd->heredoc);
		if (fd != -1)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	else if (cmd->input_file != NULL)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
		{
			perror("minishell: input file");
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

static void	handle_output(t_cmd *cmd)
{
	int	fd;

	if (cmd->output_file != NULL)
	{
		if (cmd->append == 1)
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("minishell: output file");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	handle_redirections(t_cmd *cmd)
{
	handle_input(cmd);
	handle_output(cmd);
}
