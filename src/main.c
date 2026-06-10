/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:25:08 by mwei              #+#    #+#             */
/*   Updated: 2026/06/09 17:56:47 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A dummy function to simulate the parser for now
t_command *mock_parser()
{
    // --- COMMAND 1: ls -l ---
    t_command *cmd1 = malloc(sizeof(t_command));
    cmd1->args = malloc(sizeof(char *) * 3);
    cmd1->args[0] = "ls";
    cmd1->args[1] = "-l";
    cmd1->args[2] = NULL;
    cmd1->infile = NULL; 
    cmd1->outfile = NULL;
    cmd1->append = 0;
    cmd1->heredoc = NULL;

    // --- COMMAND 2: grep ".c" ---
    t_command *cmd2 = malloc(sizeof(t_command));
    cmd2->args = malloc(sizeof(char *) * 3);
    cmd2->args[0] = "grep";
    cmd2->args[1] = "README.md"; // Escaping the dot so it looks for .c files
    cmd2->args[2] = NULL;
    cmd2->infile = NULL;
    cmd2->outfile = NULL;
    cmd2->append = 0;
    cmd2->heredoc = NULL;
    cmd2->next = NULL;

    // --- LINK THEM TOGETHER ---
    cmd1->next = cmd2;
    
    return (cmd1);
}

void execute_pipeline(t_command *cmd_list, char **envp);

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    // 1. Get the "parsed" command
    t_command *cmd_list = mock_parser();

    // 2. Pass it to your execution engine
    execute_pipeline(cmd_list, envp);

    // (Don't worry about freeing the mock memory for this quick test)
    return (0);
}