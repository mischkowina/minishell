/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builder.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 10:54:13 by vsimeono          #+#    #+#             */
/*   Updated: 2022/08/12 16:54:04 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_BUILDER_H
# define ENV_BUILDER_H

# include "../libft/libft.h"
# include "../minishell.h"

typedef struct s_data		t_data;
typedef struct s_envp_data	t_envp_data;
typedef struct s_env		t_env;

/*      Env Builder Standalone   */
t_env		*create_env_list(char **envp);
t_env		*create_env_element(char **value);
void		ft_lstadd_back_env_element(t_env **env_list, t_env *new);
void		update_shell_level(t_env *env);

#endif