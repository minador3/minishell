/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:31 by mwei              #+#    #+#             */
/*   Updated: 2026/06/17 16:50:44 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper to free the memory of a single environment node
static void free_env_node(t_env *node)
{
    if (!node)
        return;
    if (node->key)
        free(node->key);
    if (node->value)
        free(node->value);
    free(node);
}

int ft_unset(t_command *cmd, t_env **env_list)
{
    int     i = 1;
    t_env   *curr;
    t_env   *prev;

    if (!env_list || !*env_list || !cmd->args[1])
        return (1);

    // Loop through all arguments (e.g., unset VAR1 VAR2 VAR3)
    while (cmd->args[i] != NULL)
    {
        curr = *env_list;
        prev = NULL;

        while (curr != NULL)
        {
            // Check for an exact match (using the length of the list's key + 1 for the null terminator)
            if (ft_strncmp(curr->key, cmd->args[i], ft_strlen(curr->key) + 1) == 0)
            {
                if (prev == NULL) // We are deleting the very first node (head)
                    *env_list = curr->next;
                else              // We are deleting a middle or end node
                    prev->next = curr->next;
                
                free_env_node(curr);
                break; // Node removed, move to the next argument
            }
            prev = curr;
            curr = curr->next;
        }
        i++;
    }
    return (0);
}
