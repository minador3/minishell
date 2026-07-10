import subprocess
import os
import sys

# Define color constants for output formatting
GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
RESET = "\033[0m"

# Define the test cases
TEST_CASES = [
    {
        "name": "1. Basic Heredoc",
        "description": "Standard heredoc reading input and outputting it.",
        "input": [
            "cat << EOF",
            "hello world",
            "this is a simple test",
            "EOF"
        ]
    },
    {
        "name": "2. Multiple Heredoc Lines with symbols",
        "description": "Heredoc containing spaces, special characters, and empty lines.",
        "input": [
            "cat << EOF",
            "",
            "  indented line",
            "!@#$%^&*()_+",
            "EOF"
        ]
    },
    {
        "name": "3. Heredoc with environment variable expansion",
        "description": "Testing environment variable expansion ($USER, $?, and undefined variables).",
        "input": [
            "export MY_TEST_VAR=pineapple",
            "false",
            "cat << EOF",
            "my var is $MY_TEST_VAR",
            "exit code is $?",
            "undefined is $NOT_DEFINED_123",
            "EOF"
        ]
    },
    {
        "name": "4. Heredoc piped into another command",
        "description": "Heredoc input is processed by cat and then piped to grep/wc.",
        "input": [
            "cat << EOF | grep apple | wc -l",
            "apple",
            "banana",
            "apple juice",
            "pear",
            "EOF"
        ]
    },
    {
        "name": "5. Heredoc overriding pipe input (Precedence test)",
        "description": "A command has both pipe input and a heredoc. The heredoc must override the pipe.",
        "input": [
            "echo \"this pipe input should be ignored\" | cat << EOF",
            "this heredoc input should be displayed",
            "EOF"
        ]
    },
    {
        "name": "6. Pipeline with multiple heredocs (Nested heredocs)",
        "description": "First command has a heredoc, second command has its own heredoc overriding the pipe.",
        "input": [
            "cat << EOF1 | cat << EOF2",
            "input to first cat",
            "EOF1",
            "input to second cat",
            "EOF2"
        ]
    },
    {
        "name": "7. Multi-stage pipe with heredoc",
        "description": "Heredoc -> grep -> awk/wc -> cat.",
        "input": [
            "cat << EOF | grep pattern | wc -w",
            "pattern pattern",
            "no match",
            "pattern match",
            "EOF"
        ]
    },
    {
        "name": "8. Heredoc redirected to file",
        "description": "Heredoc output redirected to a file, then verified.",
        "input": [
            "cat << EOF > test_out.txt",
            "file contents from heredoc",
            "EOF",
            "cat test_out.txt",
            "rm test_out.txt"
        ]
    },
    {
        "name": "9. Heredoc with delimiter matching a command",
        "description": "Using a delimiter that is a command name ('cat' or 'echo').",
        "input": [
            "cat << cat",
            "line 1",
            "cat",
            "echo done"
        ]
    }
]

def clean_output(raw_output):
    """
    Cleans up minishell output by removing:
    - Minishell prompt ('Minishell>$ ...')
    - Heredoc prompt ('> ...')
    - Trailing 'exit' output from EOF
    """
    lines = raw_output.splitlines()
    cleaned = []
    
    for line in lines:
        # Remove prompt lines
        if line.startswith("Minishell>$"):
            continue
        if line.startswith(">"):
            continue
        if line == "exit":
            continue
        cleaned.append(line)
        
    return "\n".join(cleaned).strip()

def run_shell(binary, input_data):
    """
    Runs a shell with the given input data and returns stdout and stderr.
    """
    input_str = "\n".join(input_data) + "\n"
    try:
        proc = subprocess.run(
            [binary],
            input=input_str,
            text=True,
            capture_output=True,
            timeout=5
        )
        return proc.returncode, proc.stdout, proc.stderr
    except subprocess.TimeoutExpired:
        return -1, "", "TIMEOUT"

def main():
    print(f"{CYAN}=== Minishell Crazy Heredoc & Pipe Test Suite ==={RESET}\n")
    
    minishell_bin = "./minishell"
    if not os.path.exists(minishell_bin):
        print(f"{RED}Error: {minishell_bin} not found. Please compile first.{RESET}")
        sys.exit(1)
        
    passed_all = True
    
    for tc in TEST_CASES:
        print(f"{CYAN}Test Case: {tc['name']}{RESET}")
        print(f"Description: {tc['description']}")
        print(f"Commands run:")
        print(f"{YELLOW}" + "\n".join("  " + l for l in tc['input']) + f"{RESET}")
        
        # Run minishell
        m_code, m_stdout, m_stderr = run_shell(minishell_bin, tc['input'])
        m_cleaned = clean_output(m_stdout)
        
        # Run bash (using standard bash -c or raw stdin)
        # Note: We execute bash with standard environment but filter out prompt issues
        # We need to make sure we export/run properly in bash.
        # Since bash doesn't print prompts when stdin is a pipe, we get clean output directly.
        b_code, b_stdout, b_stderr = run_shell("bash", tc['input'])
        b_cleaned = b_stdout.strip()
        
        # Compare
        if m_cleaned == b_cleaned:
            print(f"Result: {GREEN}PASSED{RESET}\n")
        else:
            passed_all = False
            print(f"Result: {RED}FAILED{RESET}")
            print(f"--- Minishell Output (Cleaned) ---")
            print(m_cleaned)
            print(f"--- Bash Output ---")
            print(b_cleaned)
            print(f"----------------------------------\n")
            
    if passed_all:
        print(f"{GREEN}All tests passed!{RESET}")
    else:
        print(f"{RED}Some tests failed. Check output details above.{RESET}")

if __name__ == "__main__":
    main()
