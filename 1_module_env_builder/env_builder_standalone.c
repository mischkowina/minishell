/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builder_standalone.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 13:06:17 by vsimeono          #+#    #+#             */
/*   Updated: 2022/08/12 16:53:48 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_builder.h"

/**
 * Function to store the content of envp in a linked list.
 * @param envp [char **] Array of strings containing an environmental variable
 * each.
 * @return [t_env *] The first element of the new ENV list.
 */
t_env	*create_env_list(char **envp)
{
	t_env	*env_list;
	char	**array;
	int		i;

	i = 0;
	env_list = NULL;
	while (envp[i] != NULL)
	{
		array = ft_split(envp[i], '=');
		ft_lstadd_back_env_element(&env_list, create_env_element(array));
		i++;
		if (array)
		{
			free(array);
			array = NULL;
		}
	}
	update_shell_level(env_list);
	return (env_list);
}

/**
 * Function to create a list element for the ENV list from an array of
 * two strings - the variable name and the variable content.
 * @param value [char **] Array of two strings containing the name and
 * the content of an env variable.
 * @return [t_env *] List element created from the string array.
*/
t_env	*create_env_element(char **value)
{
	t_env	*element;

	element = malloc(sizeof(t_env));
	if (!element)
		return (NULL);
	element->bash_variable = value[0];
	if (value[1] == NULL)
		element->bash_v_content = NULL;
	else
		element->bash_v_content = value[1];
	element->next = NULL;
	return (element);
}

/**
 * Function to add an env element to the back of the ENV list.
 * @param env_list [t_env **] Pointer to the first element of the list.
 * @param new [t_env *] New element to be added to the end.
*/
void	ft_lstadd_back_env_element(t_env **env_list, t_env *new)
{
	t_env	*temp;

	temp = *env_list;
	if (*env_list)
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	else
		*env_list = new;
}

/**
 * Function to update the SHLVL variable to always display the number of
 * stacked shells currently running.
 * @param env [t_env *] List containing the environmental variables.
*/
void	update_shell_level(t_env *env)
{
	t_env	*tmp;
	int		i;

	tmp = get_env(env, "SHLVL");
	if (!tmp)
		return ;
	i = ft_atoi(tmp->bash_v_content);
	i++;
	free(tmp->bash_v_content);
	tmp->bash_v_content = ft_itoa(i);
}
