/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:26:53 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 19:37:39 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_parent_builtin(t_cmd *cmd, t_env **env, char **envp,
		int prev)
{
	int	original_stdout;
	int	original_stdin;
	int	status;

	if (!cmd->next && prev == -1 && cmd->argv && is_builtin(cmd->argv[0]))
	{
		original_stdout = dup(STDOUT_FILENO);
		original_stdin = dup(STDIN_FILENO);
		handle_redirections(cmd, *env);
		status = execute_builtin(cmd, envp, env);
		if (status != -2)
			update_exit_status(env, status);
		dup2(original_stdout, STDOUT_FILENO);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdout);
		close(original_stdin);
		free_envp_array(envp);
		if (status == -2)
			return (-2);
		return (1);
	}
	return (0);
}

static void	run_path_cmd(t_cmd *cmd, t_env **env, char **envp, char *path)
{
	struct stat	path_stat;

	if (!path)
	{
		if (ft_strchr(cmd->argv[0], '/') || env_get_value(*env, "PATH") == NULL)
			print_cmd_error(cmd->argv[0], ": No such file or directory");
		else
			print_cmd_error(cmd->argv[0], ": command not found");
		child_cleanup(cmd, env, envp);
		exit(127);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		print_cmd_error(path, ": Is a directory");
		free(path);
		child_cleanup(cmd, env, envp);
		exit(126);
	}
	execve(path, cmd->argv, envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(path);
	free(path);
	child_cleanup(cmd, env, envp);
	exit(126);
}

static void	execute_child(t_cmd *cmd, t_env **env, char **envp, int p[3])
{
	char	*path;
	int		ret;
	int		exit_code;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_pipes(cmd, p);
	handle_redirections(cmd, *env);
	if (cmd->argv == NULL || cmd->argv[0] == NULL)
	{
		child_cleanup(cmd, env, envp);
		exit(0);
	}
	if (is_builtin(cmd->argv[0]))
	{
		ret = execute_builtin(cmd, envp, env);
		exit_code = ret;
		if (ret == -2)
			exit_code = ft_atoi(env_get_value(*env, "?"));
		child_cleanup(cmd, env, envp);
		exit(exit_code);
	}
	path = get_path(cmd->argv[0], envp);
	run_path_cmd(cmd, env, envp, path);
}

static int	execute_single_cmd(t_cmd *cmd, t_env **env_list, int p[3],
		pid_t *pid)
{
	char	**envp;
	int		ret;

	envp = env_list_to_envp(*env_list);
	ret = handle_parent_builtin(cmd, env_list, envp, p[2]);
	if (ret == -2)
		return (-2);
	if (ret == 0)
	{
		if (cmd->next)
			pipe(p);
		*pid = fork();
		if (*pid == 0)
			execute_child(cmd, env_list, envp, p);
		handle_parent_process(cmd, p);
		free_envp_array(envp);
	}
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
	return (0);
}

int	execute_pipeline(t_cmd *cmd, t_env **env_list)
{
	int		p[3];
	pid_t	pid;
	int		ret;

	p[2] = -1;
	pid = -1;
	while (cmd)
	{
		ret = execute_single_cmd(cmd, env_list, p, &pid);
		if (ret == -2)
			return (-2);
		cmd = cmd->next;
	}
	wait_for_children(pid, env_list);
	return (0);
}
