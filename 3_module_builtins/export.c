/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 13:16:47 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 16:16:26 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * Function to reproduce the behaviour of export. In case there is no argument,
 * export prints all ENV variables in alphabetical order.
 * If there is an argument, it adds the specified variable to the ENV list and 
 * potentially also sets its value, if specified.
 * @param env [t_env *] List containing the environmental variables.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param input [char **] String array containing the arguments for export.
 * @param flag [int] Signifies if there is a pipe, since some builtins are not 
 * executed when piping.
 * @return [int] 1 at success, 0 at failure.
*/
int	ft_export(t_env *env, t_parser *parser, char **input, int flag)
{
	int		i;

	i = 1;
	if (!input[i])
		export_print(env, parser);
	if (flag == 1)
		return (1);
	else
	{
		while (input[i])
			export_handle_input(parser, env, input[i++]);
	}
	return (1);
}

/**
 * Function to parse and process an argument to export. Splits the argument
 * in two by the first '=' it occurs and initialies an array of 2 strings as 
 * input for the ENV list. In case there is no equal sign, only the first 
 * string holds values.
 * @param parser [t_parser *] Struct containing parsed input & relevant values.
 * @param env [t_env *] List containing the environmental variables.
 * @param input [char *] String representing one argument to export.
 * @return [int] 1 at success, 0 at failure.
*/
int	export_handle_input(t_parser *parser, t_env *env, char *input)
{
	char	*values[2];
	int		j;

	j = 0;
	if (input[j] == '=')
	{
		ft_error(parser, 1, NULL, "export: '=': not a valid identifier");
		return (0);
	}
	while (input[j] && input[j] != '=')
		j++;
	values[0] = ft_calloc(j + 1, sizeof(char));
	ft_memcpy(values[0], input, j);
	if (input[j++] == '=')
	{
		values[1] = ft_calloc(ft_strlen(input) - j + 1, sizeof(char));
		ft_memcpy(values[1], &input[j], ft_strlen(input) - j);
	}
	else
		values[1] = NULL;
	export_add_variable(env, values);
	return (1);
}

/**
 * Function to add or edit an ENV variable. If the variable specified by
 * the first string already exists, its content is adjusted according to
 * the input. Else, a whole new ENV variable is added at the end of the list.
 * @param env [t_env *] List containing the environmental variables.
 * @param values [char **] Two strings containing the name and the content of
 * the variable.
 * @return [int] 1 at success, 0 at failure.
*/
int	export_add_variable(t_env *env, char **values)
{
	t_env	*tmp;

	tmp = get_env(env, values[0]);
	if (tmp)
	{
		free(values[0]);
		if (tmp->bash_v_content)
			free(tmp->bash_v_content);
		tmp->bash_v_content = values[1];
	}	
	else
		ft_lstadd_back_env_element(&env, create_env_element(values));
	return (1);
}
