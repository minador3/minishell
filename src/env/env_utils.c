/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:37 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 16:21:55 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// Find node by key (exact match)
static t_env *env_find(t_env *env_list, const char *key)
{
	t_env *tmp = env_list;
	if (!key)
		return (NULL);
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

// Return the value associated with key or NULL
char *env_get_value(t_env *env_list, const char *key)
{
	t_env *node = env_find(env_list, key);
	if (!node)
		return (NULL);
	return (node->value);
}

// Set or create an environment variable. Returns 0 on success, -1 on malloc failure.
int env_set_value(t_env **env_list, const char *key, const char *value)
{
	t_env *node;
	char *new_key;
	char *new_value;

	if (!key)
		return (-1);

	node = env_find(*env_list, key);
	if (node)
	{
		// replace existing value
		if (node->value)
			free(node->value);
		if (value)
			node->value = ft_strdup(value);
		else
			node->value = NULL;
		return (0);
	}

	// create new node
	new_key = ft_strdup(key);
	if (!new_key)
		return (-1);
	if (value)
		new_value = ft_strdup(value);
	else
		new_value = NULL;
	if (value && !new_value)
	{
		free(new_key);
		return (-1);
	}
	env_add_back(env_list, new_env_node(new_key, new_value));
	return (0);
}

// Convert the linked list to a NULL-terminated envp array suitable for execve
char **env_list_to_envp(t_env *env_list)
{
	int count = 0;
	t_env *tmp = env_list;
	char **envp;
	int i = 0;

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}

	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);

	tmp = env_list;
	while (tmp)
	{
		if (tmp->value)
		{
			char *joined = ft_strjoin(tmp->key, "=");
			char *entry = ft_strjoin(joined, tmp->value);
			free(joined);
			envp[i++] = entry;
		}
		else
		{
			envp[i++] = ft_strdup(tmp->key);
		}
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

// Increments the SHLVL variable, or sets it to 1 if it doesn't exist
void update_shlvl(t_env **env_list)
{
    t_env   *temp;
    int     level;
    char    *new_level_str;

    temp = *env_list;
    while (temp != NULL)
    {
        if (ft_strncmp(temp->key, "SHLVL", 6) == 0)
        {
            level = ft_atoi(temp->value) + 1;
            new_level_str = ft_itoa(level);
            free(temp->value);
            temp->value = new_level_str;
            return;
        }
        temp = temp->next;
    }
    // If SHLVL wasn't found in the OS environment, create it
    env_add_back(env_list, new_env_node(ft_strdup("SHLVL"), ft_strdup("1")));
}
