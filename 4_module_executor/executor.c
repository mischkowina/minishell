/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 17:16:58 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 12:55:56 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * Stores the orginal STDIN and STDOUT in the parser struct.
 * Iterates through every section of the input (provided as an array of lists).
 * For each section, initializes file descriptors, prepares the execution and 
 * then calls the execution function. Afterwards, it cleans up using the
 * end_execution function.
 * @param data [t_data *] Struct containing all minishell variables.
 * @return [int] 1 at success, 0 at failure.
*/
int	executor(t_data *data)
{
	t_list		*cur_sec;
	int			i;
	t_parser	*parser;

	i = 0;
	parser = &(data->par);
	store_fds(parser);
	while (parser->sections[i])
	{
		cur_sec = parser->sections[i++];
		set_fds(parser, i);
		if (exec_prep(cur_sec, parser) == 1)
		{
			if (parser->sections[i])
				exec_section(data);
			else
				exec_last_section(data);
			free_str_array(parser->command);
		}
	}
	end_execution(parser);
	return (1);
}

/**
 * For the current input section, this function prepares the execution by 
 * identifying and opening in- and outfiles (in open_files_count_cmds) and
 * creating the command array to be handed over to execve() during execution.
 * @param sec [t_list *] List containing each word of the current input section.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @return [int] 1 at success, 0 at failure.
*/
int	exec_prep(t_list *sec, t_parser *parser)
{
	int		count;

	count = open_files_count_cmds(parser, sec);
	if (parser->input_fd < 0 || parser->output_fd < 0 || count == 0)
		return (0);
	if (create_command_array(count, parser, sec) == 0)
		return (0);
	return (1);
}

/**
 * Opens a pipe and duplicates the input_fd into STDIN, if existing.
 * Checks if the command is a builtin, if not it calls fork_section
 * to fork and execute the binary. Else, the builtin is executed.
 * Afterwards, the pipe's read end is duplicated into STDIN, in
 * case there is not outfile. Lastly, open fds are closed.
 * @param data [t_data *] Struct containing all minishell variables.
 * @return [int] 1 at success, 0 at failure.
*/
int	exec_section(t_data *data)
{
	t_parser	*parser;

	parser = &data->par;
	if (pipe(parser->pipe_fd) < 0)
		return (0);
	if (parser->input_fd > 2 && dup2(parser->input_fd, STDIN_FILENO) < 0)
	{
		ft_error(parser, 1, "dup2", "exec_section: dup2 failed");
		return (0);
	}
	if (check_builtins(data) == 0)
	{
		if (fork_section(parser, data->to_env_list) == 0)
			return (0);
	}
	if (parser->output_fd < 3
		&& dup2(parser->pipe_fd[0], parser->output_fd) < 0)
	{
		ft_error(parser, 1, "dup2", "exec_section: dup2 failed");
		return (0);
	}
	close_pipe_fd(parser);
	if (parser->output_fd > 2)
		close(parser->output_fd);
	return (1);
}

/**
 * Duplicates the infile into STDIN and the outfile into STDOUT, 
 * given there is an in- or outfile. Checks if the command is a 
 * builtin, if not it calls fork_last_section to fork and execute 
 * the binary. Else, the builtin is executed. Lastly, open fds are 
 * closed.
 * @param data [t_data *] Struct containing all minishell variables.
 * @return [int] 1 at success, 0 at failure.
*/
int	exec_last_section(t_data *data)
{
	t_parser	*parser;
	t_env		*env;

	parser = &data->par;
	env = data->to_env_list;
	if (dup2(parser->input_fd, STDIN_FILENO) < 0)
	{
		ft_error(parser, 1, "dup2", "exec_last_section: dup2 failed");
		return (0);
	}
	if (dup2(parser->output_fd, STDOUT_FILENO) < 0)
	{
		ft_error(parser, 1, "dup2", "exec_last_section: dup2 failed");
		return (0);
	}
	if (check_builtins(data) == 0)
	{
		if (fork_last_section(parser, env) == 0)
			return (0);
	}
	if (parser->input_fd > 2)
		close(parser->input_fd);
	if (parser->output_fd > 2)
		close(parser->output_fd);
	return (1);
}

/**
 * Cleanup function to restore STDIN and STDOUT to its original state and 
 * free the command line input that is not needed anymore.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @return [int] 1 at success, 0 at failure.
*/
int	end_execution(t_parser *parser)
{
	restore_std_fds(parser);
	free_lst_array(parser->sections);
	free_str_array(parser->paths);
	return (1);
}
