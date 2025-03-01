/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 14:22:40 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 14:52:06 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * Checks if a string is identical to "<" or "<<" redirections. In that case, 
 * the following list element identifies either an infile or a heredoc delimiter.
 * @param line [char *] String to be checked for the redirection characters
 * @return [int] 1 if the string was identified as redirection, else 0.
*/
int	is_infile(char *line)
{
	if (ft_strncmp(line, "<", 2) == 0)
		return (1);
	else if (ft_strncmp(line, "<<", 3) == 0)
		return (1);
	else
		return (0);
}

/**
 * Is called if file has been identified as infile or heredoc delimiter.
 * First, it checks if there was a previous, invalid infile - in that case, the
 * flag signals us to not actually use the current infile for input.
 * If there was a previous valid infile, the old one is closed.
 * Then, it diffentiates between a heredoc (<<) and calling the here_doc function
 * and a filename, for which a opening function would be called.
 * @param file [char *] Name of the infile or heredoc delimiter.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param filemode [char *] String specifying if "file" is heredoc or filename.
 * @return [int] [int] 1 at success, 0 at failure.
*/
int	infile(char *file, t_parser *parser, char *filemode)
{
	int		flag_prv_file;

	flag_prv_file = 0;
	if (parser->input_fd < 0)
		flag_prv_file = -1;
	else if (parser->input_fd > 2)
		close(parser->input_fd);
	if (ft_strncmp(filemode, "<<", 3) == 0)
		parser->input_fd = here_doc(parser, file);
	else
		parser->input_fd = open_infile(parser, file);
	if (flag_prv_file == -1 && parser->input_fd != -1)
	{
		close(parser->input_fd);
		parser->input_fd = -1;
	}
	return (1);
}

/**
 * Opens a temporary document and forks for a child process to write into it.
 * Then it opens the file again as input file for the following commands and
 * deletes it after closing the fd.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param lim [char *] String representing the delimiter, which stops user input.
 * @return [int] Returns the fd of the open heredoc, or -1 in case of error.
*/
int	here_doc(t_parser *parser, char *lim)
{
	char	*tmp;
	int		fd;
	int		test;

	tmp = NULL;
	fd = open("/tmp/.heredoc", O_WRONLY | O_CREAT, 0666);
	test = here_doc_fork(lim, tmp, fd) != 0;
	if (test != 0)
	{
		if (test == -1)
			ft_error(parser, 1, NULL, "<<: heredoc failed");
		close(fd);
		unlink("/tmp/.heredoc");
		return (-1);
	}
	if (tmp)
		free(tmp);
	if (fd >= 0)
		close(fd);
	fd = open("/tmp/.heredoc", O_RDONLY | O_CREAT, 0666);
	if (fd < 0)
		ft_error(parser, 1, NULL, "<<: heredoc failed");
	else
		unlink("/tmp/.heredoc");
	return (fd);
}

/**
 * Forks and creates a child process which reads from the command line into a
 * temporary file. In this child process, CTRL+C is executed to terminate.
 * Reads from the command line into the file, until the delimiter, EOF or SIGINT
 * is encountered. In case of SIGINT, it returns a non-zero integer.
 * @param lim [char *] String representing the delimiter, which stops user input.
 * @param tmp [char *] String holding the line read.
 * @param fd [int] File descriptor of the temporary file.
 * @return [int] Returns 0 in case of success, other integers in case of error.
*/
int	here_doc_fork(char *lim, char *tmp, int fd)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		while (1 && fd >= 0)
		{
			tmp = readline("> ");
			if (!tmp || ft_strncmp(tmp, lim, ft_strlen(tmp) + 1) == 0)
				exit(0);
			ft_putstr_fd(tmp, fd);
			ft_putchar_fd('\n', fd);
			free(tmp);
		}
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, signal_handler_parent);
	return (status);
}

/**
 * Checks if the file specified in filename is existant and readable.
 * If so, it opens the file and returns it's fd.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param filename [char *] String containing the infile's name.
 * @return [int] Returns the fd of the open infile. In case of error, returns -1.
*/
int	open_infile(t_parser *parser, char *filename)
{
	int	fd;

	fd = -1;
	if (!access(filename, F_OK) && !access(filename, R_OK))
		fd = open(filename, O_RDONLY);
	else
	{	
		if (parser->input_fd >= 0)
			ft_error(parser, 1, filename, ": No such file or directory");
	}
	return (fd);
}
