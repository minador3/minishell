/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weimin <weimin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:15:49 by mwei              #+#    #+#             */
/*   Updated: 2026/06/22 11:15:02 by weimin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
** Handles the SIGINT signal (ctrl-C).
** It prints a newline, then tells readline to clear its 
** internal buffer and draw a fresh prompt on the new line.
*/
void handle_sigint(int sig)
{
    (void)sig; // Silence the unused parameter warning
    
    // Print a newline to move the cursor down
    write(STDOUT_FILENO, "\n", 1);
    
    // Tell readline that we moved to a new line
    rl_on_new_line();
    
    // Clear the current text buffer
    rl_replace_line("", 0);
    
    // Force readline to redraw the prompt
    rl_redisplay();
}

/*
** Initializes the signal handlers for the main interactive shell.
** intercepts ctrl-C to run our custom function, and completely
** ignores ctrl-\.
*/
void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // --- 1. Setup SIGINT (ctrl-C) ---
    sa_int.sa_handler = &handle_sigint;
    sa_int.sa_flags = SA_RESTART; // Restarts interrupted system calls
    sigemptyset(&sa_int.sa_mask); // Initializes the signal set to empty
    sigaction(SIGINT, &sa_int, NULL);

    // --- 2. Setup SIGQUIT (ctrl-\) ---
    // SIG_IGN tells the OS to completely ignore this signal
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}
