/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 21:41:57 by mwei              #+#    #+#             */
/*   Updated: 2026/07/09 21:41:59 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_str(char *res, char *add)
{
	char	*new_res;

	if (!add)
		return (res);
	new_res = ft_strjoin(res, add);
	free(res);
	return (new_res);
}

static char	*expand_var(char *line, int *i, t_env *env, char *res)
{
	int		start;
	char	*temp;
	char	*val;

	(*i)++;
	start = *i;
	if (line[*i] == '?')
		(*i)++;
	else
	{
		while (ft_isalnum(line[*i]) || line[*i] == '_')
			(*i)++;
	}
	temp = ft_substr(line, start, *i - start);
	val = env_get_value(env, temp);
	free(temp);
	if (val)
		res = append_str(res, val);
	return (res);
}

static char	*append_text(char *line, int *i, char *res)
{
	int		start;
	char	*temp;

	start = *i;
	if (line[*i] == '$')
	{
		res = append_str(res, "$");
		(*i)++;
		return (res);
	}
	while (line[*i] && line[*i] != '$')
		(*i)++;
	temp = ft_substr(line, start, *i - start);
	res = append_str(res, temp);
	free(temp);
	return (res);
}

char	*expand_heredoc_line(char *line, t_env *env)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?'))
			res = expand_var(line, &i, env, res);
		else
			res = append_text(line, &i, res);
	}
	free(line);
	return (res);
}
