/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:32:29 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 16:33:30 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * Function to print all env variables in alphabetical order. First sets the 
 * output_fd either the pipe's write end or to an outfile. Then iterates through
 * ENV and gets the next smallest element, and prints it.
 * @param env [t_env *] List containing the environmental variables.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @return [int] 1 at success, 0 at failure.
*/
int	export_print(t_env *env, t_parser *parser)
{
	char	*values[2];
	t_env	*start;
	t_env	*smallest;
	int		output_fd;

	if (parser->output_fd > 2 || parser->output_fd == 1)
		output_fd = parser->output_fd;
	else
		output_fd = parser->pipe_fd[1];
	values[0] = ft_strdup("");
	values[1] = NULL;
	start = create_env_element(values);
	smallest = export_get_next_smallest(start, env);
	free(values[0]);
	free(start);
	while (smallest)
	{
		export_print_var(smallest, output_fd);
		smallest = export_get_next_smallest(smallest, env);
	}
	return (1);
}

/**
 * Function to find the next smallest element in the ENV list. Also considers
 * returning OLDPWD at the right time, in case it is not part of the list yet.
 * @param small [t_env *] The previously printed element.
 * @param env [t_env *] List containing the environmental variables.
 * @return [t_env *] The next smallest ENV element, or NULL, if small is
 * already the last element.
*/
t_env	*export_get_next_smallest(t_env *small, t_env *env)
{
	t_env	*next;
	t_env	*save;
	int		ln;

	save = env;
	ln = ft_strlen(small->bash_variable) + 1;
	while (env && ft_strncmp(small->bash_variable, env->bash_variable, ln) >= 0)
		env = env->next;
	if (!env)
		return (NULL);
	next = env;
	env = env->next;
	while (env)
	{
		ln = ft_strlen(env->bash_variable) + 1;
		if (ft_strncmp(env->bash_variable, small->bash_variable, ln) > 0)
		{
			if (ft_strncmp(env->bash_variable, next->bash_variable, ln) < 0)
				next = env;
		}
		env = env->next;
	}
	next = export_check_oldpwd(small, next, save);
	return (next);
}

/**
 * Function to make sure OLDPWD is printed, even if it is not part of the ENV
 * list yet.
 * It checks if OLDPWD lies directly between the last printed element and
 * the following element of the list. If so, it creates OLDPWD and returns 
 * it for printing.
 * On the next iteration, it checks if OLDPWD was the last printed element.
 * If it was, and it isn't part of the ENV list, it deletes the element again.
 * @param small [t_env *] The previously printed element.
 * @param next [t_env *] The next smallest ENV element.
 * @param env [t_env *] List containing the environmental variables.
 * @return [t_env *] An element containing OLDPWD, if it has to be printed at
 * this point. Else, it returns the original next element.
*/
t_env	*export_check_oldpwd(t_env *small, t_env *next, t_env *env)
{
	char	*values[2];

	if (ft_strncmp("OLDPWD", small->bash_variable, 7) > 0)
	{
		if (ft_strncmp("OLDPWD", next->bash_variable, 7) < 0)
		{
			values[0] = ft_strdup("OLDPWD");
			values[1] = NULL;
			return (create_env_element(values));
		}
	}
	if (ft_strncmp("OLDPWD", small->bash_variable, 7) == 0)
	{
		if (!get_env(env, "OLDPWD"))
		{
			free(small->bash_variable);
			free(small);
		}
	}
	return (next);
}

/**
 * Function to print a single ENV variable in the required format.
 * @param var [t_env *] The ENV element to be printed.
 * @param output_fd [int] Fd to be printed into.
 * @return [int] 1 at success, 0 at failure.
*/
int	export_print_var(t_env *var, int output_fd)
{
	ft_putstr_fd("declare -x ", output_fd);
	ft_putstr_fd(var->bash_variable, output_fd);
	if (var->bash_v_content)
	{
		ft_putstr_fd("=\"", output_fd);
		ft_putstr_fd(var->bash_v_content, output_fd);
		ft_putchar_fd('"', output_fd);
	}
	ft_putchar_fd('\n', output_fd);
	return (1);
}
