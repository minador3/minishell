#include "minishell.h"

int	types(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

void	expand_exit_status(t_word *w, int *j)
{
	char	*val;
	int		i;

	*j += 2;
	val = env_get_value(w->env_list, "?");
	if (!val)
		return ;
	i = 0;
	while (val[i])
		w->str[w->k++] = val[i++];
}

char	*extract_varname(char *line, int *j)
{
	char	*tmp;
	int		i;
	int		m;

	(*j)++;
	i = *j;
	while (ft_isalpha(line[*j]) || ft_isdigit(line[*j]) || line[*j] == '_')
		(*j)++;
	if (i == *j)
		return (NULL);
	tmp = malloc(ft_strlen(line) + 1);
	if (!tmp)
		return (NULL);
	m = 0;
	while (i < *j)
		tmp[m++] = line[i++];
	tmp[m] = '\0';
	return (tmp);
}

void	expand_variable(t_word *w, char *line, int *j)
{
	char	*tmp;
	char	*val;
	int		m;

	tmp = extract_varname(line, j);
	if (!tmp)
	{
		w->str[w->k++] = '$';
		return ;
	}
	val = env_get_value(w->env_list, tmp);
	free(tmp);
	if (!val)
		return ;
	m = 0;
	while (val[m])
		w->str[w->k++] = val[m++];
}

void	fill_word(t_word *w, char *line, int *j)
{
	while (line[*j] && (w->quote_state || !types(line[*j])))
	{
		if (w->quote_state == 0 && (line[*j] == '\'' || line[*j] == '"'))
			w->quote_state = line[*j];
		else if (w->quote_state == line[*j])
			w->quote_state = 0;
		else if (line[*j] == '$' && w->quote_state != '\'')
		{
			handle_expansion(w, line, j);
			continue ;
		}
		else
			w->str[w->k++] = line[*j];
		(*j)++;
	}
}
