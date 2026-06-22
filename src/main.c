/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 16:48:21 by weimin            #+#    #+#             */
/*   Updated: 2026/06/22 11:07:56 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A dummy function to simulate the parser for now
t_command *mock_parser()
{
    t_command *cmd = malloc(sizeof(t_command));
    
    cmd->args = malloc(sizeof(char *) * 2); 
    cmd->args[0] = "cat"; // We will feed the heredoc into cat
    cmd->args[1] = NULL;
    
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    
    // Set the delimiter! 
    cmd->heredoc = "EOF";
    cmd->next = NULL;
    
    return (cmd);
}

// ---> DELETED the old void execute_pipeline(...) line from here! <---

int main(int argc, char **argv, char **envp)
{
    t_env *my_env;

    (void)argc;
    (void)argv;

    // Initialize your dynamic environment list
    my_env = init_env(envp);

    // Get the "parsed" command
    t_command *cmd_list = mock_parser();

    // Pass the address of my_env (&my_env) so the pipeline can modify it
    execute_pipeline(cmd_list, envp, &my_env); 

    return (0);
}