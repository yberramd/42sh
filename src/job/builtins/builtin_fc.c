/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberramd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 12:34:42 by yberramd          #+#    #+#             */
/*   Updated: 2019/10/22 12:34:44 by yberramd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"
#include "libft.h"
#include "ft_getopt.h"

int		cmd_fc(int argc, char **argv)
{
	int	opt;

	g_opterr = 1;
	g_optind = RESET_OPTIND;
	while ((opt = ft_getopt(argc, argv, "rnlse:")) != -1)
	{
		if (opt == 108)/*[l]*/
			ft_printf("arg [l]\n");
		else if (opt == 115)/*[s]*/
			ft_printf("arg [s]\n");
		else if (opt == 110)/*[n]*/
			ft_printf("arg [n]\n");
		else if (opt == 114)/*[r]*/
			ft_printf("arg [r]\n");
		else if (opt == 101)/*[e]*/
			ft_printf("arg [e]\n");
		else
			ft_printf("fc: usage: fc [-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]\n");
	}
	return (0);
}