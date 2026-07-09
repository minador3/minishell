/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:03:36 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 19:23:48 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig = 0;

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

t_token	*get_tokens(char *input, t_env **env_list)
{
	t_token	*token;

	token = tokenizer(input, *env_list);
	if (token == NULL)
		return (NULL);
	if (syntax_check(token))
	{
		ft_listclear(&token);
		return (NULL);
	}
	return (token);
}

int	process_input(char *input, t_env **env_list)
{
	t_cmd	*cmd;
	t_token	*token;
	int ret;

	token = NULL;
	cmd = NULL;
	add_history(input);
	token = get_tokens(input, env_list);
	if (!token)
		return (0);
	cmd = parse(token);
	if (!cmd)
	{
		ft_listclear(&token);
		return (0);
	}
	ft_listclear(&token);
	ret = execute_pipeline(cmd, env_list);
	free_cmd(cmd);
	return (ret);
}

void	shell_loop(t_env **env_list)
{
	char	*input;

	while (1)
	{
		input = readline("Minishell>$ ");
		if (input == NULL)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (ft_strlen(input) > 0)
		{
			if (process_input(input, env_list) == -2)
			{
				free(input);
				break ;
			}
		}
		free(input);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*my_env;
	char *val;
	int code;

	(void)ac;
	(void)av;
	setup_signals();
	my_env = init_env(env);
	shell_loop(&my_env);
	/* Free readline history allocated by the readline library */
	rl_clear_history();
	val = env_get_value(my_env, "?");
	code = 0;
	if (val)
		code = ft_atoi(val);
	free_env_list(my_env);
	return (code);
}
