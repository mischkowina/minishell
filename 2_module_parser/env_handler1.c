/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 18:13:48 by smischni          #+#    #+#             */
/*   Updated: 2022/08/12 17:31:06 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
/* Find PWD from the ENV, if not there it Get's it from Memory, /
   Return NULL if Error */
static char	*get_pwd(t_env **to_env_list)
{
	char	*pwd;

	pwd = get_env_value(to_env_list, "PWD");
	if (!pwd)
		return (NULL);
	return (pwd);
}

/* Find PWD and Returns it's Value in ENV List, PWD is Taken out of Memory */
static char	*resolving_env(t_env **to_env_list, char *env_name)
{
	if (!ft_strncmp(env_name, "PWD", 4))
		return (get_pwd(to_env_list));
	else
		return (get_env_value(to_env_list, env_name));
}

/* Allocating a New String from a Value that will Replace
   the ENV Variable, Returns NULL if Error */
static char	*replace_str_env(t_data *data, char *input, int idx)
{
	char	*new_str;
	char	*env_value;
	int		length;

	length = 0;
	while (input[idx] && (ft_isalpha(input[idx])
			|| ft_isdigit(input[idx]) || input[idx] == '_'))
	{
		idx++;
		length++;
		if (!(length - 1) && ft_isdigit(input[idx - 1]))
			break ;
	}
	new_str = ft_substr(input, idx - length, length);
	if (!new_str)
		return (NULL);
	env_value = resolving_env(&data->to_env_list, new_str);
	if (new_str)
		free(new_str);
	if (!env_value)
		return (NULL);
	new_str = str_replace_str_at(input, idx - length - 1, length + 1,
			env_value);
	env_value = NULL;
	return (new_str);
}

/* Check if ENV Variable is a $? and Returns \
   it if it Finds it */
static char	*check_and_get_env(t_data *data, char *input, int idx)
{
	char	*exit_status;
	char	*ret;

	idx++;
	if (input[idx] && input[idx] == '?')
	{
		exit_status = ft_itoa(g_exit_status);
		ret = str_replace_str_at(input, idx - 1, 2, exit_status);
		if (exit_status)
			free(exit_status);
		return (ret);
	}
	else
		return (replace_str_env(data, input, idx));
}

/**
 * Function to expand ENV variables in the input to the respective content.
 * @param data [t_data *] Struct containing all minishell variables.
 * @param input [char **] String array containing the command line input.
 * @return [int] 1 or none at success, 0 at failure.
*/
int	env_resolver(t_data *data, char **input)
{
	int		i;
	char	quote;
	char	*tmp;

	quote = 0;
	i = 0;
	while ((*input)[i])
	{
		if ((!quote || quote == '"' ) && (*input)[i] == '$')
		{
			tmp = check_and_get_env(data, *input, i);
			if (!tmp)
				return (0);
			free(*input);
			*input = tmp;
			if (!(*input)[i])
				break ;
		}
		else if (!quote && ((*input)[i] == '\'' || (*input)[i] == '"'))
			quote = (*input)[i];
		else if (quote && (*input)[i] == quote)
			quote = 0;
		i++;
	}
	return (1);
}
