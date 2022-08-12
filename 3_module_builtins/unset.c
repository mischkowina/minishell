/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:04:02 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 16:37:48 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * Function to reproduce the behaviour of unset. Removes the specified variables
 * from the ENV list.
 * @param env [t_env *] List containing the environmental variables.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param flag_pipe [int] Signifies if there is a pipe, since some builtins are 
 * not executed when piping.
 * @return [int] 1 at success, 0 at failure.
*/
int	ft_unset(t_env *env, t_parser *parser, int flag_pipe)
{
	int		i;

	i = 1;
	if (flag_pipe == 1)
		return (1);
	while (parser->command[i])
	{
		if (ft_strchr(parser->command[i], '='))
		{
			ft_putstr_fd("unset: '=': not a valid identifier", 2);
			g_exit_status = 1;
		}
		else
			unset_variable(env, parser, i);
		i++;
	}
	return (1);
}

/**
 * Function to remove the ENV variable specified by the current argument from 
 * the ENV list. Makes sure that the list is not interrupted by connecting the
 * previous and the following element.
 * @param env [t_env *] List containing the environmental variables.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param i [int] Index of the current argument.
 * @return [int] 1 at success, 0 at failure.
*/
int	unset_variable(t_env *env, t_parser *parser, int i)
{
	t_env	*previous;
	t_env	*searched;
	t_env	*following;

	searched = get_env(env, parser->command[i]);
	if (searched)
	{
		following = searched->next;
		previous = get_env_previous(env, parser->command[i]);
		if (previous)
			previous->next = following;
		free(searched->bash_variable);
		free(searched->bash_v_content);
		free(searched);
	}
	return (1);
}
