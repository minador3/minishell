/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 16:48:21 by weimin            #+#    #+#             */
/*   Updated: 2026/06/22 11:19:46 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A dummy function to simulate the parser for now
t_command *mock_parser()
{
    // --- COMMAND 1: ls fake_file ---
    t_command *cmd1 = malloc(sizeof(t_command));
    cmd1->args = malloc(sizeof(char *) * 3); 
    cmd1->args[0] = "ls";
    cmd1->args[1] = "does_not_exist.txt";
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

    // We do NOT link them with a pipe this time. 
    // We want to run them sequentially to simulate hitting 'enter' twice.
    return (cmd1); // We will just return cmd1 for now
}

// ---> DELETED the old void execute_pipeline(...) line from here! <---

int main(int argc, char **argv, char **envp)
{
// 1. Get the first command (ls fake_file)
    t_command *cmd1 = mock_parser();
    execute_pipeline(cmd1, envp, &my_env); 

    // 2. Simulate typing a second command (env)
    t_command *cmd2 = malloc(sizeof(t_command));
    cmd2->args = malloc(sizeof(char *) * 2); 
    cmd2->args[0] = "env";
    cmd2->args[1] = NULL;
    cmd2->infile = NULL;
    cmd2->outfile = NULL;
    cmd2->append = 0;
    cmd2->heredoc = NULL;
    cmd2->next = NULL;
    
    // Execute the second command
    execute_pipeline(cmd2, envp, &my_env);
}
