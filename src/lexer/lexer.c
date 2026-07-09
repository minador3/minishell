/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:40 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 20:51:14 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_operator(t_token **token, char *line, int *j)
{
	t_token	*new;

	if (line[*j] == '>' && line[*j + 1] == '>')
	{
		new = ft_lexernew(APPEND, NULL);
		ft_lexeradd_back(token, new);
		*j += 2;
		return ;
	}
	else if (line[*j] == '<')
	{
		new = ft_lexernew(REDIR_IN, NULL);
		ft_lexeradd_back(token, new);
	}
	else if (line[*j] == '>')
	{
		new = ft_lexernew(REDIR_OUT, NULL);
		ft_lexeradd_back(token, new);
	}
	else if (line[*j] == '|')
	{
		new = ft_lexernew(PIPE, NULL);
		ft_lexeradd_back(token, new);
	}
	(*j)++;
}

void	handle_heredoc(t_token **token, char *line, int *j)
{
	t_token	*new;
	int		m;
	char	*tmp;

	tmp = NULL;
	*j += 2;
	while ((line[*j] >= 9 && line[*j] <= 13) || line[*j] == 32)
		(*j)++;
	tmp = malloc(ft_strlen(line) + 1);
	if (!tmp)
	{
		ft_listclear(token);
		return ;
	}
	m = 0;
	while (ft_isalpha(line[*j]) || ft_isdigit(line[*j]) || line[*j] == '_')
		tmp[m++] = line[(*j)++];
	tmp[m] = '\0';
	new = ft_lexernew(HEREDOC, tmp);
	ft_lexeradd_back(token, new);
	free(tmp);
}

void	handle_word(t_token **token, char *line, int *j, t_env *env_list)
{
	t_word	w;
	t_token	*new;

	w.k = 0;
	w.quote_state = 0;
	w.has_quotes = 0;
	w.env_list = env_list;
	w.str = malloc(ft_strlen(line) + 1);
	if (!w.str)
	{
		ft_listclear(token);
		return ;
	}
	fill_word(&w, line, j);
	w.str[w.k] = '\0';
	if (w.k > 0 || w.has_quotes)
	{
		new = ft_lexernew(WORD, w.str);
		ft_lexeradd_back(token, new);
	}
	free(w.str);
}

t_token	*tokenizer(char *line, t_env *env_list)
{
	int		j;
	t_token	*token;

	j = 0;
	token = NULL;
	while (line[j])
	{
		while (line[j] == ' ' || (line[j] >= 9 && line[j] <= 13))
			j++;
		if (!line[j])
			break ;
		if (!types(line[j]))
			handle_word(&token, line, &j, env_list);
		else if (line[j] == '<' && line[j + 1] == '<')
			handle_heredoc(&token, line, &j);
		else
			handle_operator(&token, line, &j);
	}
	return (token);
}

char	*realloc_word_buffer(char *old, int k, int new_size)
{
	char	*new_str;
	int		i;

	new_str = malloc(new_size);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < k)
	{
		new_str[i] = old[i];
		i++;
	}
	new_str[i] = '\0';
	free(old);
	return (new_str);
}
