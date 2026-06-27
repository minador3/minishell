#include "minishell.h"


int is_redirection(int type)
{
    if (type == REDIR_IN
        || type == REDIR_OUT
        || type == APPEND)
        return (1);
    return (0);
}

char *token_to_str(int type)
{
    if (type == PIPE)
        return ("|");
    if (type == REDIR_IN)
        return ("<");
    if (type == REDIR_OUT)
        return (">");
    if (type == APPEND)
        return (">>");
    if (type == HEREDOC)
        return ("<<");
    return ("unknown");
}

int syntax_check(t_token *token)
{
    t_token *head;

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
        else if (is_redirection(head->type) &&
            head->next
            && head->next->type != WORD)
        {
            printf("syntax error near unexpected token `%s`\n", 
                token_to_str(head->next->type));
            return (1);
        }
        head = head->next;
    }
    return (0);
}

int count_args(t_token *token)
{
    int i;
    t_token *head;

    head = token;
    i = 0;
    while (head != NULL)
    {
        if (head->type == WORD)
        {
            i++;
            head = head->next;
        }
        else if (head->type == REDIR_IN)
            head = head->next->next;
        else if (head->type == REDIR_OUT)
            head = head->next->next;
        else if (head->type == HEREDOC)
            head = head->next;
        else if (head->type == APPEND)
            head = head->next->next;
    }
    return (i);
}

int count_cmd_args(t_token *token)
{
    int i;
    t_token *head;

    head = token;
    i = 0;
    while (head != NULL && head->type != PIPE)
    {
        if (head->type == WORD)
        {
            i++;
            head = head->next;
        }
        else if (head->type == REDIR_IN)
            head = head->next->next;
        else if (head->type == REDIR_OUT)
            head = head->next->next;
        else if (head->type == HEREDOC)
            head = head->next;
        else if (head->type == APPEND)
            head = head->next->next;
    }
    return (i);
}

t_cmd *init_cmd(void)
{
    t_cmd *cmd;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->append = 0;
    cmd->argv = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->heredoc = NULL;
    cmd->next = NULL;
    return (cmd);
}

int allocate_cmd_arg(t_cmd *cmd, t_token *token)
{
    int count;

    count = count_cmd_args(token);
    cmd->argv = malloc(sizeof(char *) * (count + 1));
    if (!cmd->argv)
        return (1);
    return (0);
}

t_cmd *parse(t_token *token)
{
    t_token *head;
    t_cmd *head_cmd;
    t_cmd *current_cmd;
    int i;
    int count;

    count = count_cmd_args(token);
    head_cmd = init_cmd();
    if (!head_cmd)
        return (NULL);
    current_cmd = head_cmd;
    current_cmd->argv = malloc(sizeof(char *) * (count + 1));
    if (!current_cmd->argv)
    {
        free_cmd(head_cmd);
        return (NULL);
    }
    head = token;

    i = 0;
    while (head!= NULL)
    {
        if (head->type == WORD)
        {
            current_cmd->argv[i++] = ft_strdup(head->value);
            head = head->next;
        }
        else if (head->type == REDIR_IN)
        {
            current_cmd->input_file = ft_strdup(head->next->value);
            head = head->next->next;
        }
        else if (head->type == REDIR_OUT)
        {
            current_cmd->output_file = ft_strdup(head->next->value);
            head = head->next->next;
        }
        else if (head->type == APPEND)
        {
            current_cmd->output_file = ft_strdup(head->next->value);
            current_cmd->append = 1;
            head = head->next->next;
        }
        else if (head->type == HEREDOC)
        {
            if (head->value)
                current_cmd->heredoc = ft_strdup(head->value);
            head = head->next;
        }
        else if (head->type == PIPE)
        {
            current_cmd->argv[i] = NULL;
            current_cmd->next = init_cmd();
            if (!current_cmd->next)
            {
                free_cmd(head_cmd);
                return (NULL);
            }
            current_cmd = current_cmd->next;
            head = head->next;
            if (allocate_cmd_arg(current_cmd, head) == 1)
            {
                free_cmd(head_cmd);
                return (NULL);
            }
            i = 0;
        }
    }
    current_cmd->argv[i] = NULL;
    return (head_cmd);
}
