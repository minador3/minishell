/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:26:53 by mwei              #+#    #+#             */
/*   Updated: 2026/07/07 23:40:30 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_parent_builtin(t_cmd *cmd, t_env **env, char **envp,
		int prev)
{
	int original_stdout;
	int original_stdin;
	if (!cmd->next && prev == -1 && cmd->argv && is_builtin(cmd->argv[0]))
	{
		// 1. Save original file descriptors
		original_stdout = dup(STDOUT_FILENO);
		original_stdin = dup(STDIN_FILENO);
		// 2. Apply ALL redirections (input and output)
		handle_redirections(cmd, *env); 
		// 3. Execute the builtin
		update_exit_status(env, execute_builtin(cmd, envp, env));
		// 4. Restore original file descriptors
		dup2(original_stdout, STDOUT_FILENO);
		dup2(original_stdin, STDIN_FILENO);
		// 5. Close the duplicates
		close(original_stdout);
		close(original_stdin);
		
		free_envp_array(envp);
		return (1);
	}
	return (0);
}

static void	handle_parent_process(t_cmd *cmd, int p[3])
{
	if (p[2] != -1)
		close(p[2]);
	if (cmd->next != NULL)
	{
		p[2] = p[0];
		close(p[1]);
	}
}

static void	setup_child_pipes(t_cmd *cmd, int p[3])
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

static void	execute_child(t_cmd *cmd, t_env **env, char **envp, int p[3])
{
	char	*path;
	struct stat	path_stat;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_pipes(cmd, p);
	handle_redirections(cmd, *env);
	if (cmd->argv == NULL || cmd->argv[0] == NULL)
		exit(0);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, envp, env)) ;
	path = get_path(cmd->argv[0], envp);
	if (!path){
		if (ft_strchr(cmd->argv[0], '/') || env_get_value(*env, "PATH") == NULL)
			cmd_error_exit(cmd->argv[0], ": No such file or directory", NULL, 127);
		else
			cmd_error_exit(cmd->argv[0], ": command not found", NULL, 127);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		cmd_error_exit(path, ": Is a directory", path, 126);
	execve(path, cmd->argv, envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(path);
	free(path);
	exit(126);
}

void	execute_pipeline(t_cmd *cmd, t_env **env_list)
{
	int		p[3];
	pid_t	pid;
	char	**envp;

	p[2] = -1;
	pid = -1;
	while (cmd != NULL)
	{
		envp = env_list_to_envp(*env_list);
		if (handle_parent_builtin(cmd, env_list, envp, p[2]))
		{
			cmd = cmd->next;
			continue ;
		}
		if (cmd->next != NULL)
			pipe(p);
		pid = fork();
		if (pid == 0)
			execute_child(cmd, env_list, envp, p);
		handle_parent_process(cmd, p);
		free_envp_array(envp);
		cmd = cmd->next;
	}
	wait_for_children(pid, env_list);
}
