/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:18:58 by mwei              #+#    #+#             */
/*   Updated: 2026/06/29 15:07:30 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **split_arr)
{
	int	i;

	i = 0;
	if (!split_arr)
		return ;
	while (split_arr[i])
	{
		free(split_arr[i]);
		i++;
	}
	free(split_arr);
}

static char	*search_cmd_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*dir_slash;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		dir_slash = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(dir_slash, cmd);
		free(dir_slash);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	full_path = search_cmd_in_paths(paths, cmd);
	free_split(paths);
	return (full_path);
}

void	print_cmd_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
		ft_putstr_fd(cmd, STDERR_FILENO);
	if (msg)
		ft_putendl_fd(msg, STDERR_FILENO);
}
