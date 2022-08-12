/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 15:29:14 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 15:40:54 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * Function to reproduce the behaviour of echo. Initializes which fd echo
 * should write into (STDOUT or a file) and then writes into it.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @return [int] 1 at success, 0 at failure.
*/
int	ft_echo(t_parser *parser)
{
	int	output_fd;

	if (parser->output_fd > 2 || parser->output_fd == 1)
		output_fd = parser->output_fd;
	else
		output_fd = parser->pipe_fd[1];
	if (!(parser->command[1]))
		ft_putchar_fd('\n', output_fd);
	else
		echo_execute(parser, output_fd);
	return (1);
}

/**
 * Checks for a single argument whether it represents a valid flag for echo.
 * A valid flag starts with a '-' and contains only 'n' after.
 * @param str [char *] String containing an argument of echo.
 * @return [int] Returns 1 if the argument is a valid flag, else 0.
*/
int	echo_valid_flag(char *str)
{
	int	i;

	i = 1;
	if (ft_strncmp(str, "-n", 2) != 0)
		return (0);
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i])
		return (0);
	return (1);
}

/**
 * Function to print the arguments provided into the sepcified fd.
 * Skips all arguments that are valid flags and the prints all following
 * arguments. Depending on the flags, it either prints a newline or not.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param output_fd [int] Fd to be printed into.
 * @return [int] 1 at success, 0 at failure.
*/
int	echo_execute(t_parser *parser, int output_fd)
{
	int	i;
	int	flag_nl;

	i = 1;
	flag_nl = 1;
	while (echo_valid_flag(parser->command[i]) == 1)
	{
		flag_nl = 0;
		i++;
	}
	while (parser->command[i])
	{
		ft_putstr_fd(parser->command[i++], output_fd);
		if (parser->command[i])
			ft_putchar_fd(' ', output_fd);
	}
	if (flag_nl != 0)
		ft_putchar_fd('\n', output_fd);
	return (1);
}
