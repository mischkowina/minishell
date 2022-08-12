/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 14:41:51 by vsimeono          #+#    #+#             */
/*   Updated: 2022/08/12 17:24:18 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* Cleans the Command List, If All Clean then Cleans Commands as well */
// static int	free_parser(t_list **lexar, char **line)
static int	free_parser(t_data *data, t_list **lexar, char **line,
	int clean_all)
{
	if (*line)
		free(*line);
	ft_lstclear(lexar, free);
	if (clean_all)
	{
		ft_lstclear(data->par.sections, \
		(void (*)(void *))free_array);
		free_array(data->par.paths);
		data->par.paths = NULL;
	}
	return (0);
}

/* Detecting and Handling Errors in the Parser */
static int	parser_error(t_data *data, t_list **lexar, char **line,
	char *err)
{
	free_parser(data, lexar, line, 1);
	if (err)
		ft_putendl_fd(err, 2);
	else
		perror(err);
	return (0);
}

/**
 * Function to split the PATH env variable into a string array.
 * @param to_env_list [t_env **] Address of the ENV list.
 * @return [char **] String array containing PATH.
*/
static char	**get_paths_array(t_env **to_env_list)
{
	char	*path;
	char	**paths;

	path = get_env_value(to_env_list, "PATH");
	if (!path)
		return (NULL);
	else if (!ft_strlen(path))
	{
		if (path)
			path = NULL;
		return (NULL);
	}
	paths = ft_split(path, ':');
	if (path)
		path = NULL;
	return (paths);
}

/**
 * Function to parse the input. Calls functions to expand the environmental
 * variables, to remove quotes, and to split the string into the right format
 * for the executor. Afterwards, it frees the lexer list.
 * @param data [t_data *] Struct to store all minishell variables.
 * @param line [char *] String read from the command line.
 * @return [int] 1 or none at success, 0 at failure.
*/
int	parser(t_data *data, char **line)
{
	char	*tmp;

	data->par.paths = get_paths_array(&data->to_env_list);
	tmp = ft_strtrim(*line, " \v\t\f\r\n");
	free(*line);
	*line = tmp;
	if (!*line || !env_resolver(data, line))
		return (parser_error(data, &data->lexar, line, NULL));
	if (!lexer(*line, &data->lexar))
		return (parser_error(data, &data->lexar, line, NULL));
	if (!split_into_commands(data, data->lexar))
		return (free_parser(data, &data->lexar, line, 1));
	if (data->lexar)
		null_making(data->lexar);
	if (data->lexar)
		ft_lstclear(&data->lexar, free);
	if (data->lexar)
		free(data->lexar);
	return (1);
}

void	null_making(t_list *lexar)
{
	while (lexar)
	{
		lexar->line = NULL;
		lexar = lexar->next;
	}
}
