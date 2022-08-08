/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsimeono <vsimeono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 15:16:23 by smischni          #+#    #+#             */
/*   Updated: 2022/08/07 21:10:47 by vsimeono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_exit(t_env *env, t_parser *parser, int flag_pipe)
{
	t_parser	*parser;
	t_env		*env;
	int			exit_value;
	int			i;

	i = 1;
	parser = &data->to_parser_list;
	env = data->to_env_list;
	exit_value = EXIT_SUCCESS;
	if (parser->command[i++])
	{
		if (parser->command[i])
			return (0);//error handling, "exit: too many arguments"
		else if (exit_is_numeric_str(parser->command[1]) == 1)
			exit_value = ft_atoi(parser->command[1]);
		else
			ft_putstr_fd("exit: numeric argument required", 2);//tbd error message: "exit: numeric argument required"
	}
	if (flag_pipe == 1)
		return (1);
	free_str_array(parser->command);
	free_lst_array(parser->sections);
	close(parser->input_fd);
	close(parser->output_fd);
	close(parser->store_stdin);
	close(parser->store_stdout);
	close_pipe_fd(parser->pipe_fd);
	//parser itself? Anything else missing?
	ft_lstclear_env(&env, free);
	exit(exit_value);
	return (1);
}

int	exit_is_numeric_str(char *str)
{
	int		i;
	long	l;

	i = 0;
	if (str[i] == '-')
		i++;
	while(str[i] && ft_isdigit(str[i]) == 1)
		i++;
	if (str[i])
		return (0);
	l = ft_atolong(str);
	if (l > INT_MAX || l < INT_MIN)
		return (0);
	return (1);
}

long	ft_atolong(char *str)
{
	long	res;
	int		sign;
	int		i;

	sign = 1;
	res = 0;
	i = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	res = res * sign;
	return (res);
}
