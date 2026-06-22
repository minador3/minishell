/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:34 by mwei              #+#    #+#             */
/*   Updated: 2026/06/17 16:42:18 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 1. Create a new environment node
t_env *new_env_node(char *key, char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    
    // We assign the strings directly since they are already malloc'd by ft_substr
    node->key = key;
    node->value = value;
    node->next = NULL;
    
    return (node);
}

// 2. Add the node to the end of your linked list
void env_add_back(t_env **env_list, t_env *new_node)
{
    t_env *temp;

    if (!env_list || !new_node)
        return;

    if (!*env_list)
    {
        *env_list = new_node;
        return;
    }
    
    temp = *env_list;
    while (temp->next != NULL)
        temp = temp->next;
        
    temp->next = new_node;
}

// 3. The main extraction logic
t_env *init_env(char **envp)
{
    t_env   *env_list = NULL;
    int     i = 0;
    char    *key;
    char    *value;
    char    *equals_sign;
    int     key_len;

    if (!envp)
        return (NULL);

    while (envp[i] != NULL)
    {
        // Find the memory address of the first '=' character
        equals_sign = ft_strchr(envp[i], '=');

        if (equals_sign != NULL)
        {
            // Pointer arithmetic! Subtracting the start address from the '=' address gives the length
            key_len = equals_sign - envp[i];
            
            // Extract the key
            key = ft_substr(envp[i], 0, key_len);
            
            // Extract the value (starting 1 character after the '=')
            value = ft_substr(equals_sign + 1, 0, ft_strlen(equals_sign + 1));
            
            env_add_back(&env_list, new_env_node(key, value));
        }
        else
        {
            // Edge case: A variable without an '=' (e.g., exported without a value)
            key = ft_strdup(envp[i]);
            env_add_back(&env_list, new_env_node(key, NULL));
        }
        i++;
    }
    return (env_list);
}
