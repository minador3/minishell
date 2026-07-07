/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:22 by mwei              #+#    #+#             */
/*   Updated: 2026/07/07 17:29:42 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env **env_list)
{
	t_env	*temp;

	if (!env_list || !*env_list)
		return (0);
	temp = *env_list;
	while (temp != NULL)
	{
		if (temp->value != NULL)
		{
			ft_putstr_fd(temp->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(temp->value, STDOUT_FILENO);
		}
		temp = temp->next;
	}
	return (0);
}
