/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:25:08 by mwei              #+#    #+#             */
/*   Updated: 2026/06/10 16:37:13 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A dummy function to simulate the parser for now
t_command *mock_parser()
{
    t_command *cmd = malloc(sizeof(t_command));
    
    cmd->args = malloc(sizeof(char *) * 2); 
    cmd->args[0] = "env";
    cmd->args[1] = NULL;
    
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    cmd->heredoc = NULL;
    cmd->next = NULL;
    
    return (cmd);
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