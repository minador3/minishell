/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 16:48:21 by weimin            #+#    #+#             */
/*   Updated: 2026/06/22 11:00:04 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A dummy function to simulate the parser for now
t_command *mock_parser()
{
    // --- COMMAND 1: export NEW_VAR=42_Abu_Dhabi ---
    t_command *cmd1 = malloc(sizeof(t_command));
    cmd1->args = malloc(sizeof(char *) * 3); 
    cmd1->args[0] = "export";
    cmd1->args[1] = "NEW_VAR=42_Abu_Dhabi";
    cmd1->args[2] = NULL;
    cmd1->infile = NULL;
    cmd1->outfile = NULL;
    cmd1->append = 0;
    cmd1->heredoc = NULL;

    // --- COMMAND 2: env ---
    t_command *cmd2 = malloc(sizeof(t_command));
    cmd2->args = malloc(sizeof(char *) * 2); 
    cmd2->args[0] = "env";
    cmd2->args[1] = NULL;
    cmd2->infile = NULL;
    cmd2->outfile = NULL;
    cmd2->append = 0;
    cmd2->heredoc = NULL;
    cmd2->next = NULL;

    // Link them!
    cmd1->next = cmd2;
    
    return (cmd1);
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