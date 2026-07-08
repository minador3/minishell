/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:24 by mwei              #+#    #+#             */
/*   Updated: 2026/07/08 14:39:21 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_cmd *cmd, t_env **env)
{
	int	code;

	printf("exit\n");
	if (cmd->argv[1])
	{
		if (!is_numeric(cmd->argv[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n",
				cmd->argv[1]);
			update_exit_status(env, 255);
			return (-2);
		}
		if (cmd->argv[2])
		{
			printf("minishell: exit: too many arguments\n");
			return (1);
		}
		code = ft_atoi(cmd->argv[1]);
	}
	else
		code = ft_atoi(env_get_value(*env, "?"));
	update_exit_status(env, code);
	return (-2);
}
