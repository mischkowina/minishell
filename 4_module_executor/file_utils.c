/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 18:18:55 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 13:37:17 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * Checks the elements of the current section for redirections. If there is a
 * redirection, the following element is used to open the respective file.
 * All elements that are neither identified as files nor as redirections are
 * counted, to allocate enough space for the command string array.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param sec [t_list *] List containing each word of the current input section.
 * @return [int] Number of all list elements that are commands, options or 
 * command arguments
*/
int	open_files_count_cmds(t_parser *parser, t_list *sec)
{
	int		count;
	t_list	*head;

	count = 0;
	head = sec;
	while (head)
	{
		if (is_infile(head->line) == 1 && head->next)
		{
			infile((char *)head->next->line, parser, (char *)head->line);
			head = head->next;
		}
		else if (is_outfile(head->line) == 1 && head->next)
		{
			outfile((char *)head->next->line, parser, (char *)head->line);
			head = head->next;
		}
		else
			count++;
		head = head->next;
	}
	return (count);
}

/**
 * Initializes the fds to standard values before preparing the execution of a 
 * section. Sets the input_fd to STDIN and the outout_fd to either STDIN (if 
 * there is a following section requiring to pipe) or STDOUT (if there is no
 * following section). Also initialies the pipe_fds to 0, to make sure there
 * are no old values stored in the pipe.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param i [int] Index of the following section.
 * @return [int] 1 at success, 0 at failure.
*/
int	set_fds(t_parser *parser, int i)
{
	parser->input_fd = STDIN_FILENO;
	if (parser->sections[i])
		parser->output_fd = STDIN_FILENO;
	else
		parser->output_fd = STDOUT_FILENO;
	parser->pipe_fd[0] = 0;
	parser->pipe_fd[1] = 0;
	return (1);
}

/**
 * Duplicates the original STDIN and STDOUT and stores them in the struct, 
 * to be able to restore them after execution.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @return [int] 1 at success, 0 at failure.
*/
int	store_fds(t_parser *parser)
{
	parser->store_stdin = dup(STDIN_FILENO);
	parser->store_stdout = dup(STDOUT_FILENO);
	return (1);
}

/**
 * Restores the original STDIN and STDOUT from the duplicated fds stored in
 * the struct.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @return [int] 1 at success, 0 at failure.
*/
int	restore_std_fds(t_parser *parser)
{
	if (dup2(parser->store_stdin, 0) < 0 || dup2(parser->store_stdout, 1) < 0)
	{
		close(parser->store_stdin);
		close(parser->store_stdout);
		return (0);
	}
	close(parser->store_stdin);
	close(parser->store_stdout);
	return (1);
}

/**
 * Closes the pipe_fds, if necessary, and sets them back to 0.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @return [int] 1 at success, 0 at failure.
*/
int	close_pipe_fd(t_parser *parser)
{
	if (parser->pipe_fd[0] > 2)
		close(parser->pipe_fd[0]);
	if (parser->pipe_fd[1] > 2)
		close(parser->pipe_fd[1]);
	parser->pipe_fd[0] = 0;
	parser->pipe_fd[1] = 0;
	return (1);
}
