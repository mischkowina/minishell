/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:37:06 by smischni          #+#    #+#             */
/*   Updated: 2022/08/05 11:15:26 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../minishell.h"

typedef struct s_data	t_data;
typedef struct s_parser	t_parser;
typedef struct s_env	t_env;

int		executor(t_data *data);
int		exec_prep(t_list *sec, t_parser *parser);
int		exec_section(t_data *data);
int		exec_last_section(t_data *data);

int		is_infile(char *line);
int		infile(char *file, t_parser *parser, char *filemode);
int		here_doc(char *lim);
int		open_infile(char *filename);

int		is_outfile(char *line);
int		outfile(char *file, t_parser *parser, char *filemode);

int		create_command_array(int count, t_parser *parser, t_list *sec);
int		add_path(t_parser *parser);

int		free_str_array(char **str);
int		free_lst_array(t_list **lists);
int		close_pipe_fd(int fd[2]);
char	**reassemble_env(t_env *env);
int		store_fds(t_parser *parser);
int		restore_std_fds(t_parser *parser);

//only for testing
int		print_all_input(t_data *data);

#endif