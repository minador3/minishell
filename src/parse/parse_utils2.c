/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 21:42:09 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 21:42:11 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_tokens(t_token *head)
{
	if (head->type == PIPE && head->next == NULL)
	{
		printf("syntax error near unexpected token `newline`\n");
		return (1);
	}
	else if (head->type == PIPE && head->next && head->next->type == PIPE)
	{
		printf("syntax error near unexpected token'|'\n");
		return (1);
	}
	else if (is_redirection(head->type) && head->next == NULL)
	{
		printf("syntax error near unexpected token `newline`\n");
		return (1);
	}
	else if (is_redirection(head->type)
		&& head->next
		&& head->next->type != WORD)
	{
		printf("syntax error near unexpected token `%s`\n",
			token_to_str(head->next->type));
		return (1);
	}
	return (0);
}

int	syntax_check(t_token *token)
{
	t_token	*head;

	if (!token)
		return (0);
	head = token;
	if (token->type == PIPE)
	{
		printf("syntax error near unexpected token '|'\n");
		return (1);
	}
	while (head != NULL)
	{
		if (check_tokens(head))
			return (1);
		head = head->next;
	}
	return (0);
}
