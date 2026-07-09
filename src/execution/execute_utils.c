/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:26:53 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 19:37:39 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_exit_status(t_env **env_list, int status)
{
	char	*status_str;
	t_env	*temp;

	status_str = ft_itoa(status);
	if (!status_str)
		return ;
	temp = *env_list;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->key, "?", 2) == 0)
		{
			if (temp->value)
				free(temp->value);
			temp->value = status_str;
			return ;
		}
		temp = temp->next;
	}
	env_add_back(env_list, new_env_node(ft_strdup("?"), status_str));
}

void	wait_for_children(pid_t last_pid, t_env **env_list)
{
	int	status;
	int	final;

	final = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (last_pid != -1 && last_pid != 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			final = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			final = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit (core dumped)\n");
			else if (WTERMSIG(status) == SIGINT)
				printf("\n");
		}
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	setup_signals();
	update_exit_status(env_list, final);
}

void	setup_child_pipes(t_cmd *cmd, int p[3])
{
	if (p[2] != -1)
	{
		dup2(p[2], STDIN_FILENO);
		close(p[2]);
	}
	if (cmd->next != NULL)
	{
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
		close(p[0]);
	}
}

void	child_cleanup(t_cmd *cmd, t_env **env, char **envp)
{
	free_cmd(cmd);
	free_envp_array(envp);
	if (env && *env)
		free_env_list(*env);
}

void	handle_parent_process(t_cmd *cmd, int p[3])
{
	if (p[2] != -1)
		close(p[2]);
	if (cmd->next != NULL)
	{
		p[2] = p[0];
		close(p[1]);
	}
}
