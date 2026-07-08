/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:00:51 by mwei              #+#    #+#             */
/*   Updated: 2026/07/07 19:13:27 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
		ft_putstr_fd(cmd, STDERR_FILENO);
	if (msg)
		ft_putendl_fd(msg, STDERR_FILENO);
}

void	cmd_error_exit(char *cmd, char *msg, char *path, int code)
{
	print_cmd_error(cmd, msg);
	if (path)
		free(path);
	exit(code);
}

// 1. Condense the exit status updater
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
			free(temp->value);
			temp->value = status_str;
			return ;
		}
		temp = temp->next;
	}
	env_add_back(env_list, new_env_node(ft_strdup("?"), status_str));
}

// 2. Helper to free the char ** array to save lines in the main loop
void	free_envp_array(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

// 3. Isolated waiting logic
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
