/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:03:36 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 17:27:25 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_sig = 0;

void	free_env_list(t_env *env_list)
{
	t_env	*temp;

	while (env_list != NULL)
	{
		temp = env_list->next;
		if (env_list->key)
			free(env_list->key);
		if (env_list->value)
			free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}

void	free_cmd_list(t_command *cmd_list)
{
	t_command	*temp;
	int			i;

	while (cmd_list != NULL)
	{
		temp = cmd_list->next;
		if (cmd_list->args)
		{
			i = 0;
			while (cmd_list->args[i])
				free(cmd_list->args[i++]);
			free(cmd_list->args);
		}
		if (cmd_list->infile)
			free(cmd_list->infile);
		if (cmd_list->outfile)
			free(cmd_list->outfile);
		if (cmd_list->heredoc)
			free(cmd_list->heredoc);
		free(cmd_list);
		cmd_list = temp;
	}
}

// Minified temporary parser
t_command	*parse_input(char *line, t_env **env_list)
{
	char		**tokens;
	int			count;
	int			i;
	t_command	*cmd;

	(void)env_list;
	count = 0;
	tokens = ft_split(line, ' ');
	if (!tokens)
		return (NULL);
	while (tokens[count])
		count++;
	if (count == 0)
		return (free(tokens), NULL);
	cmd = malloc(sizeof(t_command));
	cmd->args = malloc(sizeof(char *) * (count + 1));
	i = -1;
	while (++i < count)
	{
		cmd->args[i] = ft_strdup(tokens[i]);
		free(tokens[i]);
	}
	cmd->args[i] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	return (free(tokens), cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_env		*my_env;
	char		*line;
	t_command	*cmd_list;

	(void)argc;
	(void)argv;
	setup_signals();
	my_env = init_env(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL && printf("exit\n"))
			break ;
		if (*line != '\0')
		{
			add_history(line);
			cmd_list = parse_input(line, &my_env);
			if (cmd_list != NULL)
			{
				execute_pipeline(cmd_list, &my_env);
				free_cmd_list(cmd_list);
			}
		}
		free(line);
	}
	free_env_list(my_env);
	return (0);
}
