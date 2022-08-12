/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smischni <smischni@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:30:36 by vsimeono          #+#    #+#             */
/*   Updated: 2022/08/12 14:59:49 by smischni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

/**
 * In case SIGINT occured, a new prompt is given and the line is not added to
 * the history.
 * @param signum [int] Number specifying which signal occured.
*/
void	signal_handler_parent(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\b\b\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
