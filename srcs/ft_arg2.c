/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 19:48:21 by mchardin          #+#    #+#             */
/*   Updated: 2020/01/10 21:13:03 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int		is_end_of_command(t_separator sep)
{
	if (sep == END_LINE || sep == SEMICOLON || sep == PIPE)
		return (1);
	return (0);
}

// int is_redirection(t_separator sep)
// {
//     if (sep == TO_END || sep == TO_FILE || sep == FROM_FILE)
//         return (1);
//     return (0);
// }

int		init_struct(t_shell *shell)
{
	if (!(shell->tab = malloc(sizeof(char*) * 2)))
		return (0);
	if (!(set_arg(shell)))
		return (0);
	shell->tab[0] = shell->arg.str;
	shell->tab[1] = 0;
	shell->output = 0;
	shell->fd = 1;
	return (1);
}

int		ft_redirection(t_shell *shell, t_separator prev)
{
	if (prev == FROM_FILE)
		return (1);
	if (shell->fd != 1)
		close(shell->fd);
	if (prev == TO_FILE &&
		(shell->fd = open(shell->arg.str, O_WRONLY, O_CREAT)) < 0)
	{
		// PRINTF ERROR
		return (0);
	}
	else if ((shell->fd = open(shell->arg.str, O_WRONLY, O_CREAT)) < 0)
	free(shell->arg.str);
}

int		analyse_args(t_shell *shell)
{
	t_separator prev;

	if (!(init_struct(shell)))
		return (0);
	while (!is_end_of_command(shell->arg.sep))
	{
		prev = shell->arg.sep;
		if (!(set_arg(shell)))
		{
			ft_free_strs(shell->tab);
			return (0);
		}
		if (prev == ARG &&
			!(shell->tab = ft_strs_plus_one(shell->tab, shell->arg.str)))
			return (0);
		else if (!(ft_redirection(shell, prev)))
			return (0);
	}
}