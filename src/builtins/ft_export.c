/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:27 by mwei              #+#    #+#             */
/*   Updated: 2026/06/22 10:57:38 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper: Bash variables must start with a letter or underscore,
// and only contain letters, numbers, or underscores.
static int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] != '=' && str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Helper: Extracts the key/value and adds/updates the linked list
static void	add_or_update_env(t_env **env_list, char *arg)
{
	char	*key;
	char	*value;
	char	*equals_sign;
	t_env	*temp;

	// 1. Split the argument safely using the first '='
	equals_sign = ft_strchr(arg, '=');
	if (equals_sign)
	{
		key = ft_substr(arg, 0, equals_sign - arg);
		value = ft_substr(equals_sign + 1, 0, ft_strlen(equals_sign + 1));
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL; // No equals sign means no value (e.g., export VAR)
	}
	// 2. Check if the key already exists
	temp = *env_list;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
		{
			// Key exists! Update value ONLY if a new one was provided
			if (value != NULL)
			{
				if (temp->value)
					free(temp->value);
				temp->value = value;
			}
			free(key); // We didn't use the new key string, so free it
			return ;
		}
		temp = temp->next;
	}
	// 3. If we get here, the key didn't exist. Add it to the end!
	env_add_back(env_list, new_env_node(key, value));
}

// The Main Builtin
void	ft_export(t_command *cmd, t_env **env_list)
{
	int		i;
	t_env	*temp;

	i = 1;
	// 1. No arguments: Print the environment
	if (cmd->args[1] == NULL)
	{
		temp = *env_list;
		while (temp != NULL)
		{
			if (temp->value)
				printf("declare -x %s=\"%s\"\n", temp->key, temp->value);
			else
				printf("declare -x %s\n", temp->key);
			temp = temp->next;
		}
		return ;
	}
	// 2. Loop through arguments and add/update them
	while (cmd->args[i] != NULL)
	{
		if (!is_valid_identifier(cmd->args[i]))
			printf("minishell: export: `%s': not a valid identifier\n",
				cmd->args[i]);
		else
			add_or_update_env(env_list, cmd->args[i]);
		i++;
	}
}
