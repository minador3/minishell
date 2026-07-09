#include "minishell.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// Global required by the shell
int	g_sig = 0;

// Re-implemented helper from main.c since we exclude main.o
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

// Redirects stdout to a pipe, runs fn, restores stdout, and asserts the content
static void assert_stdout(int (*fn)(void *), void *arg, const char *expected)
{
	int pipefd[2];
	char buffer[4096];
	int saved_stdout = dup(STDOUT_FILENO);

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);

	fn(arg);

	fflush(stdout);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	int n = read(pipefd[0], buffer, sizeof(buffer) - 1);
	if (n < 0)
		n = 0;
	buffer[n] = '\0';
	close(pipefd[0]);

	assert(strcmp(buffer, expected) == 0);
}

static int run_echo(void *arg)
{
	return (ft_echo((t_cmd *)arg));
}

static int run_pwd(void *arg)
{
	(void)arg;
	return (ft_pwd());
}

static void test_env(void)
{
	char *envp[] = {"PATH=/bin:/usr/bin", "USER=test_user", "SHLVL=1", NULL};
	t_env *env = init_env(envp);

	assert(env != NULL);
	assert(strcmp(env_get_value(env, "USER"), "test_user") == 0);
	assert(strcmp(env_get_value(env, "PATH"), "/bin:/usr/bin") == 0);
	// SHLVL is incremented by update_shlvl to 2 during init_env
	assert(strcmp(env_get_value(env, "SHLVL"), "2") == 0);

	// Test setting value
	assert(env_set_value(&env, "USER", "new_val") == 0);
	assert(strcmp(env_get_value(env, "USER"), "new_val") == 0);

	// Test adding new value
	assert(env_set_value(&env, "NEW_VAR", "hello") == 0);
	assert(strcmp(env_get_value(env, "NEW_VAR"), "hello") == 0);

	// Test env_list_to_envp filters out internal "?" and valueless vars
	env_add_back(&env, new_env_node(ft_strdup("NO_VAL_VAR"), NULL));
	char **converted = env_list_to_envp(env);
	assert(converted != NULL);
	int i = 0;
	int found_no_val = 0;
	int found_q = 0;
	while (converted[i])
	{
		if (strstr(converted[i], "NO_VAL_VAR") != NULL)
			found_no_val = 1;
		if (strstr(converted[i], "?") != NULL)
			found_q = 1;
		i++;
	}
	assert(found_no_val == 0);
	assert(found_q == 0);
	free_envp_array(converted);

	free_env_list(env);
	printf("test_env: PASSED\n");
}

static void test_lexer(void)
{
	char *envp[] = {"USER=test_user", "?=0", NULL};
	t_env *env = init_env(envp);

	printf("  test_lexer: simple command...\n");
	t_token *toks = tokenizer("ls -la", env);
	assert(toks != NULL);
	assert(toks->type == WORD);
	assert(strcmp(toks->value, "ls") == 0);
	assert(toks->next != NULL);
	assert(toks->next->type == WORD);
	assert(strcmp(toks->next->value, "-la") == 0);
	assert(toks->next->next == NULL);
	ft_listclear(&toks);

	printf("  test_lexer: operators...\n");
	toks = tokenizer("cat < input.txt | grep hello >> out.txt", env);
	t_token *curr = toks;
	assert(curr != NULL && curr->type == WORD && strcmp(curr->value, "cat") == 0); curr = curr->next;
	assert(curr != NULL && curr->type == REDIR_IN && curr->value == NULL); curr = curr->next;
	assert(curr != NULL && curr->type == WORD && strcmp(curr->value, "input.txt") == 0); curr = curr->next;
	assert(curr != NULL && curr->type == PIPE && curr->value == NULL); curr = curr->next;
	assert(curr != NULL && curr->type == WORD && strcmp(curr->value, "grep") == 0); curr = curr->next;
	assert(curr != NULL && curr->type == WORD && strcmp(curr->value, "hello") == 0); curr = curr->next;
	assert(curr != NULL && curr->type == APPEND && curr->value == NULL); curr = curr->next;
	assert(curr != NULL && curr->type == WORD && strcmp(curr->value, "out.txt") == 0); curr = curr->next;
	assert(curr == NULL);
	ft_listclear(&toks);

	printf("  test_lexer: expansion...\n");
	toks = tokenizer("echo $USER", env);
	assert(toks != NULL && toks->next != NULL);
	assert(strcmp(toks->next->value, "test_user") == 0);
	ft_listclear(&toks);

	printf("  test_lexer: single quotes...\n");
	toks = tokenizer("echo '$USER'", env);
	assert(toks != NULL && toks->next != NULL);
	assert(strcmp(toks->next->value, "$USER") == 0);
	ft_listclear(&toks);

	printf("  test_lexer: double quotes...\n");
	toks = tokenizer("echo \"$USER\"", env);
	assert(toks != NULL && toks->next != NULL);
	assert(strcmp(toks->next->value, "test_user") == 0);
	ft_listclear(&toks);

	printf("  test_lexer: strict unquoted empty expansion discard...\n");
	toks = tokenizer("ls $VAR_THAT_DOES_NOT_EXIST", env);
	assert(toks != NULL);
	assert(toks->type == WORD);
	assert(strcmp(toks->value, "ls") == 0);
	assert(toks->next == NULL);
	ft_listclear(&toks);

	free_env_list(env);
	printf("test_lexer: PASSED\n");
}

static void test_parser(void)
{
	char *envp[] = {"USER=test_user", NULL};
	t_env *env = init_env(envp);

	// Command parsing
	t_token *toks = tokenizer("ls -la", env);
	t_cmd *cmd = parse(toks);
	assert(cmd != NULL);
	assert(cmd->argv != NULL);
	assert(strcmp(cmd->argv[0], "ls") == 0);
	assert(strcmp(cmd->argv[1], "-la") == 0);
	assert(cmd->argv[2] == NULL);
	assert(cmd->input_file == NULL);
	assert(cmd->output_file == NULL);
	assert(cmd->heredoc == NULL);
	assert(cmd->next == NULL);
	free_cmd(cmd);
	ft_listclear(&toks);

	// Redirections parsing
	toks = tokenizer("cat < input.txt > output.txt", env);
	cmd = parse(toks);
	assert(cmd != NULL);
	assert(strcmp(cmd->argv[0], "cat") == 0);
	assert(cmd->argv[1] == NULL);
	assert(strcmp(cmd->input_file, "input.txt") == 0);
	assert(strcmp(cmd->output_file, "output.txt") == 0);
	assert(cmd->append == 0);
	free_cmd(cmd);
	ft_listclear(&toks);

	// Append redirection parsing
	toks = tokenizer("echo hello >> append.txt", env);
	cmd = parse(toks);
	assert(cmd != NULL);
	assert(strcmp(cmd->argv[0], "echo") == 0);
	assert(strcmp(cmd->argv[1], "hello") == 0);
	assert(strcmp(cmd->output_file, "append.txt") == 0);
	assert(cmd->append == 1);
	free_cmd(cmd);
	ft_listclear(&toks);

	// Heredoc parsing
	toks = tokenizer("grep hello << EOF", env);
	cmd = parse(toks);
	assert(cmd != NULL);
	assert(strcmp(cmd->argv[0], "grep") == 0);
	assert(strcmp(cmd->argv[1], "hello") == 0);
	assert(strcmp(cmd->heredoc, "EOF") == 0);
	assert(cmd->heredoc_fd == -1);
	free_cmd(cmd);
	ft_listclear(&toks);

	// Pipeline parsing
	toks = tokenizer("cat file | grep text | wc -l", env);
	cmd = parse(toks);
	assert(cmd != NULL);
	assert(strcmp(cmd->argv[0], "cat") == 0);
	assert(cmd->next != NULL);
	assert(strcmp(cmd->next->argv[0], "grep") == 0);
	assert(cmd->next->next != NULL);
	assert(strcmp(cmd->next->next->argv[0], "wc") == 0);
	assert(cmd->next->next->next == NULL);
	free_cmd(cmd);
	ft_listclear(&toks);

	free_env_list(env);
	printf("test_parser: PASSED\n");
}

static void test_builtins(void)
{
	char *envp[] = {"USER=test_user", NULL};
	t_env *env = init_env(envp);

	// Test ft_echo
	t_token *toks = tokenizer("echo hello world", env);
	t_cmd *cmd = parse(toks);
	assert_stdout(run_echo, cmd, "hello world\n");
	free_cmd(cmd);
	ft_listclear(&toks);

	// Test ft_echo -n
	toks = tokenizer("echo -n no_newline", env);
	cmd = parse(toks);
	assert_stdout(run_echo, cmd, "no_newline");
	free_cmd(cmd);
	ft_listclear(&toks);

	// Test ft_pwd
	char cwd[1024];
	assert(getcwd(cwd, sizeof(cwd)) != NULL);
	char expected_pwd[1024];
	strcpy(expected_pwd, cwd);
	strcat(expected_pwd, "\n");
	assert_stdout(run_pwd, NULL, expected_pwd);

	// Test ft_cd updates PWD and OLDPWD
	char orig_cwd[1024];
	assert(getcwd(orig_cwd, sizeof(orig_cwd)) != NULL);
	t_token *cd_toks = tokenizer("cd ..", env);
	t_cmd *cd_cmd = parse(cd_toks);
	assert(ft_cd(cd_cmd, &env) == 0);
	char new_cwd[1024];
	assert(getcwd(new_cwd, sizeof(new_cwd)) != NULL);
	assert(strcmp(env_get_value(env, "PWD"), new_cwd) == 0);
	assert(strcmp(env_get_value(env, "OLDPWD"), orig_cwd) == 0);
	assert(chdir(orig_cwd) == 0); // Restore original dir
	free_cmd(cd_cmd);
	ft_listclear(&cd_toks);

	// Test ft_unset rejects invalid identifiers
	t_token *unset_toks = tokenizer("unset 123_invalid", env);
	t_cmd *unset_cmd = parse(unset_toks);
	assert(ft_unset(unset_cmd, &env) == 1);
	free_cmd(unset_cmd);
	ft_listclear(&unset_toks);

	// Test ft_unset protects special exit status "?"
	assert(env_get_value(env, "?") != NULL);
	unset_toks = tokenizer("unset ?", env);
	unset_cmd = parse(unset_toks);
	ft_unset(unset_cmd, &env);
	assert(env_get_value(env, "?") != NULL); // Must still exist
	free_cmd(unset_cmd);
	ft_listclear(&unset_toks);

	free_env_list(env);
	printf("test_builtins: PASSED\n");
}

static void test_path_resolver(void)
{
	char *envp[] = {"PATH=/bin:/usr/bin", NULL};
	char *path1 = get_path("ls", envp);
	assert(path1 != NULL);
	assert(strcmp(path1, "/bin/ls") == 0 || strcmp(path1, "/usr/bin/ls") == 0);
	free(path1);

	char *path2 = get_path("/bin/ls", envp);
	assert(path2 != NULL);
	assert(strcmp(path2, "/bin/ls") == 0);
	free(path2);

	char *path3 = get_path("nonexistent_command_12345", envp);
	assert(path3 == NULL);

	printf("test_path_resolver: PASSED\n");
}

int main(void)
{
	printf("=== Running Minishell Asserts-Only Test Suite ===\n");
	test_env();
	test_lexer();
	test_parser();
	test_builtins();
	test_path_resolver();
	printf("=== All tests PASSED successfully! ===\n");
	return (0);
}
