/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 17:18:01 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 16:03:12 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * Function to reproduce the behaviour of exit. Prints exit to the STDOUT
 * and terminates the shell, after freeing all allocated memory and closing all
 * open fds. Exits with either the integer specified in the first argument, or
 * with the current value of the global exit status.
 * @param data [t_data *] Struct containing all minishell variables.
 * @param flag_pipe [int] Signifies if there is a pipe, since some builtins 
 * are not executed when piping.
 * @return [int] 1 or none at success, 0 at failure.
*/
int	ft_exit(t_data *data, int flag_pipe)
{
	t_parser	*parser;
	int			exit_value;

	parser = &data->par;
	exit_value = g_exit_status;
	ft_putstr_fd("exit\n", parser->store_stdout);
	if (parser->command[1])
	{
		if (parser->command[2])
		{
			ft_error(parser, 1, NULL, "exit: too many arguments");
			return (0);
		}
		else if (exit_is_numeric_str(parser->command[1]) == 1)
			exit_value = ft_atoi(parser->command[1]);
		else
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			exit_value = 255;
		}
	}
	if (flag_pipe == 1)
		return (1);
	at_exit(data, parser, data->to_env_list);
	exit(exit_value);
}

/**
 * Function to free all remaining allocated memory and close all open fds.
 * @param data [t_data *] Struct containing all minishell variables.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param env [t_env *] List containing the environmental variables.
 * @return [int] 1 at success, 0 at failure.
*/
int	at_exit(t_data *data, t_parser *parser, t_env *env)
{
	free_str_array(parser->command);
	free_str_array(parser->paths);
	free_lst_array(parser->sections);
	if (data->line)
		free(data->line);
	close(parser->input_fd);
	close(parser->output_fd);
	close(parser->store_stdin);
	close(parser->store_stdout);
	close_pipe_fd(parser);
	if (env)
		ft_lstclear_env(&env, free);
	if (data)
		free(data);
	return (1);
}

/**
 * Checks if the argument handed over in str is fully numeric.
 * @param str [char *] String containing an argument of exit.
 * @return [int] 1 if it is numeric, 0 if not.
*/
int	exit_is_numeric_str(char *str)
{
	int		i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]) == 1)
		i++;
	if (str[i])
		return (0);
	return (1);
}
