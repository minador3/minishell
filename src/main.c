/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:03:36 by mwei              #+#    #+#             */
/*   Updated: 2026/06/23 17:27:25 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_sig = 0;

void	free_env_list(t_env *env_list)
{
	t_env	*temp;

	while (env_list != NULL)
	{
		temp = env_list->next;
		if (env_list->key)
			free(env_list->key);
		if (env_list->value)
			free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}

void	shell_loop(t_env **env_list)
{
	char *input;
    t_token *token;
    t_cmd *cmd;

	while (1)
	{
        input = readline("Minishell>$ ");
        if (input == NULL)
            exit(0);
        if (strlen(input) > 0)
        {
            token = NULL;
            cmd = NULL;
            add_history(input);
            token = tokenizer(input, *env_list);
            if (token == NULL)
            {
                free(input);
                continue;
            }
            if (syntax_check(token))
            {
                free(input);
                ft_listclear(&token);
                continue;
            }
            cmd = parse(token);
            if (!cmd)
            {
                free(input);
                ft_listclear(&token);
                continue;
            }
            execute_pipeline(cmd, env_list);
            free_cmd(cmd);
            ft_listclear(&token);
        }
	    free(input);
	}
}

int main(int ac, char **av, char **env)
{
	t_env		*my_env;

	(void)ac;
	(void)av;
	setup_signals();              // from her main
    my_env = init_env(env);      // from her main
    shell_loop(&my_env);          // yours, but now takes t_env **
    free_env_list(my_env);        // from her main
    return (0);
}