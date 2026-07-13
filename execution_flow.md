# Minishell Execution Flow & Process Management Documentation

This document explains in detail how Minishell executes parsed commands. It covers heredoc preprocessing, process piping and forking, environment conversions, file descriptor redirections, path resolution, parent vs. child built-ins, and exit status collection.

---

## 1. Execution Architecture Overview

Once the parser builds a command linked list (`t_cmd`), Minishell executes the pipeline. 

Minishell supports two main execution modes:
1. **Parent Built-in Execution:** For a **single command** that is a shell built-in (e.g., `cd`, `exit`, `export`, `unset`), execution must happen in the main parent shell process. This allows changes to the shell's state (environment variables, working directory) to persist.
2. **Pipeline Execution:** For external programs (like `ls`, `grep`, `cat`) or multiple piped commands (e.g., `ls | grep txt`), Minishell spawns child processes via `fork`, establishes pipes between them, configures redirections, and executes them concurrently using `execve` or child built-ins.

```
                  process_input (main.c)
                         │
              1. preprocess_heredocs (heredoc.c)
                         │
              2. execute_pipeline (execute.c)
                         │
         ┌───────────────┴───────────────┐
         ▼                               ▼
 [ Single Built-in ]            [ Pipeline/External ]
  Executed in Parent             Forked Child Processes
  - Redirections applied         - Pipe channels created
  - Built-in runs                - Redirections applied
  - Exit code set in "?"         - Execve / Child built-in runs
  - Redirections restored        - Parent waits for children
```

---

## 2. Step-by-Step Execution Control Flow

### Step 2.1: Heredoc Preprocessing
Before executing any commands, Minishell pre-processes all heredocs to avoid blocking during process execution.
1. **[preprocess_heredocs](file:///home/mwei/42Projects/mini_sub/src/execution/heredoc.c#L73)** iterates over the commands list.
2. If `cmd->heredoc` (the delimiter string) is present, it invokes **[process_heredoc](file:///home/mwei/42Projects/mini_sub/src/execution/heredoc.c#L45)**:
   - Sets up a standard UNIX pipe: `pipe(fd)`.
   - Registers a signal event hook (`heredoc_sig_check`) to detect if the user cancels via `SIGINT` (Ctrl+C).
   - Enters a loop reading lines with `readline("> ")`.
   - If the input line is `NULL`, matches the delimiter, or if `SIGINT` is caught, the loop breaks.
   - Otherwise, calls **[write_line_to_pipe](file:///home/mwei/42Projects/mini_sub/src/execution/heredoc.c#L22)**:
     - Expands environment variables inside the line using **[expand_heredoc_line](file:///home/mwei/42Projects/mini_sub/src/execution/heredoc_utils.c#L77)**.
     - Writes the expanded text followed by a newline (`\n`) to `fd[1]` (the write-end of the pipe).
   - Closes the write-end `fd[1]`.
   - If canceled via `SIGINT`, it closes the read-end `fd[0]`, sets exit code to `130`, and returns `-1` (which aborts execution).
   - Otherwise, returns the read-end `fd[0]` and stores it in `cmd->heredoc_fd`.

---

### Step 2.2: Pipeline Loop Orchestration

**[execute_pipeline](file:///home/mwei/42Projects/mini_sub/src/execution/execute.c#L127)** executes the commands sequentially. It maintains an array `int p[3]` to manage pipe file descriptors:
* `p[0]`: Current pipe read-end (input to next command).
* `p[1]`: Current pipe write-end (output of current command).
* `p[2]`: Previous pipe read-end (input to current command, initialized to `-1`).

For each `t_cmd` node:
1. Calls **[execute_single_cmd](file:///home/mwei/42Projects/mini_sub/src/execution/execute.c#L99)**:
   - Converts the `t_env` linked list into a `char **envp` array using **[env_list_to_envp](file:///home/mwei/42Projects/mini_sub/src/env/env_utils1.c)** for `execve`.
   - Checks for parent built-ins using **[handle_parent_builtin](file:///home/mwei/42Projects/mini_sub/src/execution/execute.c#L15)**.
2. If it is NOT a parent built-in:
   - Spawns a pipe if there is a next command in the pipeline: `pipe(p)`.
   - Forks a child process: `fork()`.
   - **Parent Process:**
     - Calls **[handle_parent_process](file:///home/mwei/42Projects/mini_sub/src/execution/execute_utils.c#L89)**:
       - Closes the previous pipe's read-end `p[2]` (since the current command finished reading from it).
       - If there is a next command, updates `p[2] = p[0]` (saving the read-end of the current pipe for the next command) and closes the write-end `p[1]`.
     - Frees the temporary `envp` array.
   - **Child Process:**
     - Calls **[execute_child](file:///home/mwei/42Projects/mini_sub/src/execution/execute.c#L71)**.
3. If the command's `heredoc_fd` is open, it closes it.
4. Advances to `cmd->next`.

---

### Step 2.3: Child Process Execution

Inside the child process (**[execute_child](file:///home/mwei/42Projects/mini_sub/src/execution/execute.c#L71)**):
1. **Signals:** Resets `SIGINT` and `SIGQUIT` signal handlers to their default behavior (`SIG_DFL`).
2. **Piping:** Calls **[setup_child_pipes](file:///home/mwei/42Projects/mini_sub/src/execution/execute_utils.c#L66)**:
   - If there is a previous pipe read-end (`p[2] != -1`), redirects standard input from it: `dup2(p[2], STDIN_FILENO)` and closes `p[2]`.
   - If there is a next command, redirects standard output to the current write-end: `dup2(p[1], STDOUT_FILENO)`, then closes both `p[1]` and `p[0]`.
3. **Redirections:** Calls **[handle_redirections](file:///home/mwei/42Projects/mini_sub/src/execution/redirections.c#L59)**:
   - **Input redirection:** If a heredoc is present (`heredoc_fd != -1`), duplicates it to `STDIN_FILENO`. Otherwise, if an `input_file` is defined, opens it `O_RDONLY` and duplicates the file descriptor to `STDIN_FILENO`.
   - **Output redirection:** If an `output_file` is defined, opens it with write flags (`O_WRONLY | O_CREAT`). If `append` is set, uses `O_APPEND`; otherwise, uses `O_TRUNC`. Duplicates the file descriptor to `STDOUT_FILENO`.
4. **Execution:**
   - If the command is a built-in, runs **[execute_builtin](file:///home/mwei/42Projects/mini_sub/src/builtins/builtin_dispatcher.c#L36)** and exits the child process with the return value.
   - Otherwise, resolves path using **[get_path](file:///home/mwei/42Projects/mini_sub/src/execution/path_resolver.c#L39)**.
   - Invokes **[run_path_cmd](file:///home/mwei/42Projects/mini_sub/src/execution/execute.c#L42)**:
     - Checks if the command is a directory (exits with status `126`).
     - Calls `execve(path, cmd->argv, envp)`.
     - If `execve` fails, displays an error and exits with status `126`.
     - If command path cannot be resolved, exits with status `127` ("command not found").

---

### Step 2.4: Clean up and exit status collection

After the pipeline loop finishes spawning all commands, the parent shell calls **[wait_for_children](file:///home/mwei/42Projects/mini_sub/src/execution/execute_utils.c#L38)**:
1. Ignores `SIGINT` and `SIGQUIT` temporarily.
2. Waits for the last command child process using `waitpid(last_pid, &status, 0)`.
   - If the child exited normally, retrieves its status with `WEXITSTATUS(status)`.
   - If terminated by a signal (e.g. Ctrl+C or Ctrl+\\), calculates exit status as `128 + WTERMSIG(status)`. It prints appropriate messages (like `Quit (core dumped)` for `SIGQUIT`).
3. Cleans up all remaining zombie child processes: `while (waitpid(-1, NULL, 0) > 0);`.
4. Re-enables parent signal handling.
5. Saves this final status into the env variable `?` via **[update_exit_status](file:///home/mwei/42Projects/mini_sub/src/execution/execute_utils.c#L15)**.

---

## 3. Visualizing Pipeline Redirections & Piping

When executing `cmd1 | cmd2 | cmd3`:

### Step 1: `cmd1`
* Previous pipe: `p[2] = -1` (uses standard stdin).
* Spawns pipe `p`.
* Forks child 1:
  - Redirects `stdout` to write-end `p[1]`.
* Parent: closes write-end `p[1]`, updates `p[2] = p[0]` (holds read-end of pipe 1).

```
   [ cmd1 Child ] 
     STDOUT ────► p[1] (Write) ====( Pipe 1 )==== p[0] (Read) [ Saved in p[2] ]
```

### Step 2: `cmd2`
* Previous pipe: `p[2]` holds read-end of Pipe 1.
* Spawns new pipe `p`.
* Forks child 2:
  - Redirects `stdin` from `p[2]` (Pipe 1 read-end).
  - Redirects `stdout` to write-end `p[1]` (Pipe 2 write-end).
* Parent: closes old read-end `p[2]`, closes write-end `p[1]`, updates `p[2] = p[0]` (holds read-end of Pipe 2).

```
   p[2] (Read Pipe 1) ────► STDIN ─── [ cmd2 Child ] ─── STDOUT ────► p[1] (Write Pipe 2)
```

### Step 3: `cmd3`
* Previous pipe: `p[2]` holds read-end of Pipe 2.
* No next command: `pipe` is not called.
* Forks child 3:
  - Redirects `stdin` from `p[2]` (Pipe 2 read-end).
  - Keeps normal `stdout`.
* Parent: closes old read-end `p[2]`.

```
   p[2] (Read Pipe 2) ────► STDIN ─── [ cmd3 Child ] ─── STDOUT ────► Standard Terminal Output
```

---

## 4. Parent Built-in Execution Flow

If a command is `cd`, `exit`, `export` (with arguments), or `unset`, and it is **not** part of a pipeline, it executes in the parent process:

1. **Dup standard descriptors:**
   - `original_stdout = dup(STDOUT_FILENO)`
   - `original_stdin = dup(STDIN_FILENO)`
2. **Apply redirections:** Calls `handle_redirections(cmd, *env_list)` to temporarily redirect file descriptors (e.g., if writing built-in output to a file: `echo "hello" > file`).
3. **Execute:** Calls `execute_builtin(cmd, env_list)`.
4. **Restore standard descriptors:**
   - `dup2(original_stdout, STDOUT_FILENO)`
   - `dup2(original_stdin, STDIN_FILENO)`
   - Closes the temporary duped descriptors.
5. **Update Exit Status:** Saves the status into the `?` environment variable.
