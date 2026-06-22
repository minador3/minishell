/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 16:48:21 by weimin            #+#    #+#             */
/*   Updated: 2026/06/22 15:54:37 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Minimal parser: split the input line by spaces into a single command
t_command *parse_input(char *line, t_env **env_list)
{
    char **tokens;
    int i;
    int count = 0;
    t_command *cmd;

    (void)env_list;
    tokens = ft_split(line, ' ');
    if (!tokens)
        return (NULL);

    while (tokens[count])
        count++;
    if (count == 0)
    {
        free(tokens);
        return (NULL);
    }

    cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        i = 0;
        while (tokens[i])
            free(tokens[i++]);
        free(tokens);
        return (NULL);
    }
    cmd->args = malloc(sizeof(char *) * (count + 1));
    if (!cmd->args)
    {
        free(cmd);
        i = 0;
        while (tokens[i])
            free(tokens[i++]);
        free(tokens);
        return (NULL);
    }

    i = 0;
    while (i < count)
    {
        cmd->args[i] = ft_strdup(tokens[i]);
        free(tokens[i]);
        i++;
    }
    free(tokens);
    cmd->args[i] = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    cmd->heredoc = NULL;
    cmd->next = NULL;
    return (cmd);
}

// Free a linked list of t_command and all allocated internals
static void free_cmd_list(t_command *head)
{
    t_command *tmp;
    int i;

    while (head)
    {
        tmp = head->next;
        if (head->args)
        {
            i = 0;
            while (head->args[i])
            {
                free(head->args[i]);
                i++;
            }
            free(head->args);
        }
        if (head->infile)
            free(head->infile);
        if (head->outfile)
            free(head->outfile);
        if (head->heredoc)
            free(head->heredoc);
        free(head);
        head = tmp;
    }
}

int main(int argc, char **argv, char **envp)
{
    t_env       *my_env;
    char        *line;
    t_command   *cmd_list;

    (void)argc;
    (void)argv;

    // 1. Initialize system
    setup_signals();
    my_env = init_env(envp);

    // 2. The Main Interactive Loop
    while (1)
    {
        // A. Display the prompt and wait for the user to type
        line = readline("minishell$ ");

        // B. Handle ctrl-D (EOF). Readline returns NULL if ctrl-D is pressed.
        if (line == NULL)
        {
            printf("exit\n"); // Bash prints exit on ctrl-D
            break;
        }

        // C. If the user actually typed something (not just hitting Enter)
        if (*line != '\0')
        {
            // Add it to the bash history so they can use the up/down arrows
            add_history(line);

            // --- INTEGRATION POINT ---
            // This is where your partner's code comes in! 
            // It will look something like: cmd_list = parse_input(line, &my_env);
            
            cmd_list = mock_parser(); // Using the mock for now so it compiles

            // D. Execute the parsed commands
            if (cmd_list != NULL)
            {
                execute_pipeline(cmd_list, &my_env);
                // Free the parsed command list to avoid leaking memory
                free_cmd_list(cmd_list);
            }
        }
        
        // Readline mallocs the 'line' string, so we must free it at the end of every loop
        free(line); 
    }

    // TODO: Free your my_env linked list here before the program completely exits
    return (0);
}
