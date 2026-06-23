/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:22 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 17:16:56 by mwei             ###   ########.fr       */
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
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	return (0);
}
