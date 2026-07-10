# Minishell

Minishell is a lightweight, simplified command-line interpreter re-implementing a subset of a POSIX-compliant shell (like Bash). This project is part of the systems programming curriculum at **School 42**.

---

## Features

* **Interactive Command Line**: Displays a custom prompt (`Minishell>$ `) and maintains command history using the GNU Readline library.
* **Command Execution**: Locates and runs binaries using the `PATH` environment variable, as well as handling absolute and relative paths.
* **Builtin Commands**: 
  * `echo` (with `-n` option support)
  * `cd` (with relative or absolute paths)
  * `pwd` (prints current working directory)
  * `export` (sets environment variables)
  * `unset` (removes environment variables)
  * `env` (prints environment variables)
  * `exit` (exits the shell with status code support)
* **Redirections**:
  * `<` : Redirects standard input from a file.
  * `>` : Redirects standard output to a file (truncates existing content).
  * `>>` : Redirects standard output to a file (appends to existing content).
  * `<< [DELIMITER]` : Reads heredoc input, supports variable expansion inside the heredoc, and exits on the exact delimiter.
* **Pipes (`|`)**: Chains commands by connecting the stdout of one command to the stdin of the next.
* **Environment Variables**: Expands environment variables (e.g., `$USER`) and handles the special parameter `$?` to retrieve the exit status of the most recently executed pipeline.
* **Signal Handling**: Handles standard signals in both interactive and non-interactive contexts:
  * `Ctrl-C` : Interrupts execution or prompts a new line.
  * `Ctrl-D` : Signals End-of-File (EOF) to exit the shell.
  * `Ctrl-\` : Does nothing in the interactive prompt, but triggers core dumps for executing children.

---

## Architecture & Pipeline Flow

The shell operates in a continuous read-eval-print loop (REPL). When you type a command and press Enter, it goes through the following pipeline:

```mermaid
    input["User Input String"] --> lexer["1. Lexer (Tokens)"]
    lexer --> parser["2. Parser (Command Nodes)"]
    parser --> heredoc["3. Heredoc Prep"]
    heredoc --> executor["4. Executor (Fork & Pipes)"]
    executor --> redirect["5. Redirections"]
    redirect --> execve["6. execve / Builtins"]
    execve --> status["7. Wait & Update $?"]
```

### Walkthrough of a Concrete Example:
`cat << EOF | grep "hello" > output.txt`

1. **Lexer / Tokenizer (1)**: Cuts the raw string into tokens:
   `[WORD: "cat"]`, `[HEREDOC: "EOF"]`, `[PIPE: "|"]`, `[WORD: "grep"]`, `[WORD: "hello"]`, `[REDIR_OUT: ">"]`, `[WORD: "output.txt"]`.
2. **Parser (2)**: Validates syntax and groups the tokens into two command nodes:
   * **Command 1**: Executes `cat`, expects a heredoc delimited by `"EOF"`.
   * **Command 2**: Executes `grep "hello"`, redirects its output to `"output.txt"`.
3. **Heredoc Preprocessing (3)**: Prompts the user to enter text for `EOF` *before* running any commands.
4. **Executor & Pipes (4)**: Creates a pipe (`|`) to connect Command 1's output to Command 2's input, and forks a child process for each command.
5. **Redirections (5)**: 
   * Command 1 redirects `stdin` to read from the heredoc data, and `stdout` to the pipe.
   * Command 2 redirects `stdin` to read from the pipe, and `stdout` to `"output.txt"`.
6. **Execution (6)**: Runs the commands (`cat` and `grep`) via `execve`.
7. **Wait & Exit Status (7)**: The parent shell waits for the last command (`grep`) to finish, and updates the exit code `$?`.

---

### Data Structures & Linked Lists

A core design decision in this project is the use of linear linked lists to manage data dynamically. Memory overhead is kept low, and traversal is simple:

* **Environment Variables (`t_env`)**: Stored in a linear linked list. This allows easy lookup, insertion (for `export`), and deletion (for `unset`).
* **Tokens (`t_token`)**: The lexer tokenizes the command line string into a linear linked list of tokens.
* **Command Pipeline (`t_cmd`)**: The parser organizes the tokens into a linear linked list of commands, where each node represents a command in the pipeline (separated by `|`).

---

### Division of Labor

* **Parsing (Implemented by placeholder)**:
  * **Lexer / Tokenizer**: Converts the input string into a list of tokens, expanding environment variables and managing quoting states.
  * **Parser / Syntax Checker**: Analyzes the token list for syntax errors (e.g. consecutive pipes) and compiles them into a command linked list (`t_cmd`).

* **Execution (Implemented by mwei)**:
  * **Heredoc Preprocessing**: Reads all heredoc prompts and caches the inputs in temporary pipe descriptors.
  * **Pipeline Executor**: Forks child processes, chains pipes, handles file redirections, and executes builtins or binaries via `execve`.
  * **Signal Handler**: Configures shell interrupt responses (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`).

---

## Instructions

### Building the Project
To compile the shell, run:
```bash
make
```
This generates the `./minishell` executable in the root directory.

### Running Minishell
Start the interactive shell:
```bash
./minishell
```

---

## Usage of AI in development

This codebase was audited and refined with the help of **Google Antigravity** (Gemini 3.5 Flash). AI was used for:

1. **Finding and Fixing Critical Bugs**:
   * **Infinite Loop in Heredoc Expansion:** Found a major bug where typing a literal `$` followed by a non-alphanumeric character (e.g. `!@#$%^&*()_+`) inside a heredoc would trap the parser loop. The AI diagnosed the lack of index advancement in `append_text` and patched it to consume the character correctly.
2. **Documentation and Code Review**:
   * Validated memory leak suppression parameters for readline under `valgrind` and assisted in building comprehensive documentation.

---

## Resources & System Calls

Understanding standard Unix mechanisms is core to building a shell:
* **Processes**: `fork`, `execve`, `wait`, `waitpid`, `exit`.
* **Files & Streams**: `open`, `close`, `read`, `write`, `dup`, `dup2`, `pipe`.
* **Interprocess Communication**: [Unix Pipes (pipe(2) man page)](https://man7.org/linux/man-pages/man2/pipe.2.html)
* **Readline Library**: [GNU Readline documentation](https://tiswww.case.edu/php/chet/readline/rltop.html)
