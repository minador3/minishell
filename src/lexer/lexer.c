#include "minishell.h"

int types(char c)
{
    if (c == '|' || c == '>' || c == '<' || c == ' ')
        return (1);
    return (0);
}


t_token *tokenizer(char *line, t_env *env_list)
{
    int i;
    int j;
    int m;
    int k;
    char *str;
    char *exit_str;
    char *tmp;
    char quote_state;
    t_token *token = NULL;
    t_token *new;

    i = 0;
    j = 0;
    quote_state = 0;
    exit_str = NULL;
    tmp = NULL;
    while (line[j])
    {
        while (line[j] == ' ')
            j++;
        if (!line[j])
            break;
        if (!types(line[j]))
        {
            k = 0;
            quote_state = 0;

            str = malloc(ft_strlen(line) + 1);
            if (!str)
            {
                ft_listclear(&token);
                return (NULL);
            }
            while (line[j] && (quote_state || !types(line[j])))
            {
                if (quote_state == 0 && (line[j] == '\'' || line[j] == '"'))
                    quote_state = line[j];
                else if (quote_state == line[j])
                    quote_state = 0;
                else if (line[j] == '$' && quote_state != '\'')
                {
                    if (line[j+1] == '?')
                    {
                        j += 2;
                        exit_str = env_get_value(env_list, "?");
                        if (exit_str)
                        {
                            i = 0;
                            while (exit_str[i])
                                str[k++] = exit_str[i++];
                        }
                        continue;
                    }
                    else
                    {
                        j++;
                        i = j;
                        while (ft_isalpha(line[j]) || ft_isdigit(line[j]) || line[j] == '_')
                            j++;
                        if (i == j)
                        {
                            str[k++] = '$';
                            continue;
                        }
                        tmp = malloc(ft_strlen(line) + 1);
                        if (!tmp)
                        {
                            free(str);
                            ft_listclear(&token);
                            return (NULL);
                        }
                        m = 0;
                        while (i < j)
                            tmp[m++] = line[i++];
                        tmp[m] = '\0';
                        exit_str = env_get_value(env_list, tmp);
                        if (exit_str)
                        {
                            m = 0;
                            while (exit_str[m])
                                str[k++] = exit_str[m++];
                        }
                        free(tmp);
                        continue;
                    }
                }
                else
                    str[k++] = line[j];
                j++;
            }
            str[k] = '\0';
            new = ft_lexernew(WORD, str);
            ft_lexeradd_back(&token, new);
            free(str);
        }
        else if (line[j] == '>' && line[j+1] == '>')
        {
            new = ft_lexernew(APPEND, NULL);
            ft_lexeradd_back(&token, new);
            j += 2;
        }
        else if (line[j] == '<' && line[j+1] == '<')
        {
            j += 2;
            while ((line[j] >= 9 && line[j] <= 13) || line[j] == 32)
                j++;
            tmp = malloc(ft_strlen(line) + 1);
            if (!tmp)
            {
                ft_listclear(&token);
                return (NULL);
            }
            m = 0;
            while (ft_isalpha(line[j]) || ft_isdigit(line[j]) || line[j] == '_')
                tmp[m++] = line[j++];
            tmp[m] = '\0';
            new = ft_lexernew(HEREDOC, tmp);
            ft_lexeradd_back(&token, new);
            free(tmp);
        }
        else if (line[j] == '>')
        {
            new = ft_lexernew(REDIR_OUT, NULL);
            ft_lexeradd_back(&token, new);
            j++;
        }
        else if (line[j] == '<')
        {
            new = ft_lexernew(REDIR_IN, NULL);
            ft_lexeradd_back(&token, new);
            j++;
        }
        else if (line[j] == '|')
        {
            new = ft_lexernew(PIPE, NULL);
            ft_lexeradd_back(&token, new);
            j++;
        }
    }
    return (token);
}