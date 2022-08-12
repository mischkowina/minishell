/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:30:02 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 15:58:06 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * Checks if parser->command[0] identifies a builtin and if so, executes it.
 * Afterwards closes the pipe's write-end, if the output was written to the
 * pipe.
 * @param data [t_data *] Struct containing all minishell variables.
 * @return [int] 1 if a builtin was identified and executed, else 0
*/
int	check_builtins(t_data *data)
{
	int			flag_pipe;
	t_parser	*parser;
	t_env		*env;

	parser = &data->par;
	env = data->to_env_list;
	flag_pipe = 0;
	if (parser->sections[1])
		flag_pipe = 1;
	if (bi_switch(data, parser, env, flag_pipe) == 0)
		return (0);
	else
	{
		if (parser->pipe_fd[1] > 2)
			close(parser->pipe_fd[1]);
		return (1);
	}
}

/**
 * Checks if parser->command[0] identifies a builtin and if so, calls
 * the respective function to execute it.
 * @param data [t_data *] Struct containing all minishell variables.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param env [t_env *] List containing the environmental variables.
 * @param flag [int] Signifies if there is a pipe, since some builtins are
 * not executed when piping.
 * @return [int] 1 if a builtin was identified and executed, else 0
*/
int	bi_switch(t_data *data, t_parser *parser, t_env *env, int flag)
{
	int	len;
	int	rtrn;

	len = ft_strlen(parser->command[0]);
	if (len == 0)
		return (0);
	if (ft_strncmp(parser->command[0], "cd", len) == 0)
		rtrn = ft_cd(env, parser, flag);
	else if (ft_strncmp(parser->command[0], "echo", len) == 0)
		rtrn = ft_echo(parser);
	else if (ft_strncmp(parser->command[0], "env", len) == 0)
		rtrn = ft_env(env, parser);
	else if (ft_strncmp(parser->command[0], "exit", len) == 0)
		rtrn = ft_exit(data, flag);
	else if (ft_strncmp(parser->command[0], "export", len) == 0)
		rtrn = ft_export(env, parser, parser->command, flag);
	else if (ft_strncmp(parser->command[0], "pwd", len) == 0)
		rtrn = ft_pwd(parser);
	else if (ft_strncmp(parser->command[0], "unset", len) == 0)
		rtrn = ft_unset(env, parser, flag);
	else
		return (0);
	if (rtrn == 1)
		g_exit_status = 0;
	return (1);
}

/**
 * Iterates through the list of environmental variables looking for the 
 * variable name specified in v_name. If that variable exists, it is 
 * returned - else the function returns NULL.
 * @param env [t_env *] List containing the environmental variables.
 * @param v_name [char *] Name of the searched variable.
 * @return [t_env *] List element containing the searched variable.
*/
t_env	*get_env(t_env *env, char *v_name)
{
	while (env)
	{
		if (ft_strncmp(env->bash_variable, v_name, ft_strlen(v_name) + 1) == 0)
		{
			return (env);
		}
		else
			env = env->next;
	}
	return (NULL);
}

/**
 * Iterates through the list of environmental variables looking for the 
 * variable name specified in v_name. If that variable exists, the element 
 * previous to that is returned - else the function returns NULL.
 * @param env [t_env *] List containing the environmental variables.
 * @param v_name [char *] Name of the searched variable.
 * @return [t_env *] List element containing the searched variable.
*/
t_env	*get_env_previous(t_env *env, char *v_name)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->bash_variable, v_name, ft_strlen(v_name) + 1) == 0)
			break ;
		tmp = tmp->next;
		count++;
	}
	if (count == 0)
		return (NULL);
	count--;
	while (count)
	{
		count--;
		env = env->next;
	}
	return (env);
}

/**
 * Checks if the specifed command identifies a builtin.
 * @param cmd [char *] Command to be checked.
 * @return [int] 1 if it is a builtin, 0 if it isn't.
*/
int	is_builtin(char	*cmd)
{
	int	len;

	len = ft_strlen(cmd);
	if (ft_strncmp(cmd, "cd", len) == 0)
		return (1);
	else if (ft_strncmp(cmd, "echo", len) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", len) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", len) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", len) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", len) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", len) == 0)
		return (1);
	else
		return (0);
}
