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

#include <unistd.h>
#include <getopt.h>

#include "history.h"
#include "libft.h"
#include "ft_getopt.h"

extern char	*optarg;
extern int	optind;
extern int	optopt;
extern int	opterr;
extern int	optreset;

static int		ft_atoi_history(const char *str)
{
	int				i;
	unsigned long	nbr;
	unsigned short	val;

	i = 0;
	nbr = 0;
	while (str[i] == ' ' || (str[i] > 8 && str[i] < 14))
		++i;
	while (str[i] > 47 && str[i] < 58)
	{
		val = str[i] ^ ((1 << 5) | (1 << 4));
		nbr = nbr * 10 + val;
		if (nbr > 500)
			return (501);
		++i;
	}
	return (nbr);
}

static int		ft_execute()
{
	ft_printf("fc -s\n");
	return (1);
}

static int		ft_fc()
{
	ft_printf("fc\n");
	return (1);
}

static int		ft_print_history(int arg, int index, char **argv)
{
	int		i;
	int		max;
	int		nbr;
	char	*cmd;

	i = 1;
	max = 1;
	(void)arg;
	history(FIRST, NULL, &cmd);
	while (history(FORWARD, NULL, &cmd) != 2 && cmd)
		max++;
	if (argv[index])
	{
		nbr = ft_atoi_history(argv[index]);
		max -= nbr;
		history(FIRST, NULL, &cmd);
		if (history(GET, NULL, &cmd) && (i - max) > 0)
			ft_printf("\t%d\t%s\n", i, cmd);
		while (history(FORWARD, NULL, &cmd) != 2 && cmd)
		{
			i++;
			if ((i - max) > 0)
				ft_printf("\t%d\t%s\n", i, cmd);
		}
	}
	else
	{
		max -= 16;
		history(FIRST, NULL, &cmd);
		if (history(GET, NULL, &cmd) && (i - max) > 0)
		{
			if (!(arg & ARG_N))
				ft_printf("%d", i);
			ft_printf("\t%s\n", cmd);// ARG_R
		}
		while (history(FORWARD, NULL, &cmd) != 2 && cmd)
		{
			i++;
			if (!(arg & ARG_N) && (i - max) > 0)
				ft_printf("%d", i);
			if ((i - max) > 0)
				ft_printf("\t%s\n", cmd);
		}
	}
	return (1);
}

int		cmd_fc(int argc, char **argv)
{
	int	opt;
	int	arg;
	int	index;

	index = 1;
	arg = 0;
	optind = RESET_OPTIND;
	opterr = 1;
	while ((opt = getopt(argc, argv, "rnlse:")) != -1)// A REMPLACER PAR FT_GETOPT DE ANTOINE
	{
		if (opt == 108)/*[l]*/
			arg = arg | ARG_L;
		else if (opt == 115)/*[s]*/
			arg = arg | ARG_S;
		else if (opt == 110)/*[n]*/
			arg = arg | ARG_N;
		else if (opt == 114)/*[r]*/
			arg = arg | ARG_R;
		else if (opt == 101)/*[e]*/
			arg = arg | ARG_E;
		else
		{
			ft_printf("fc: usage: fc [-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]\n");
			return (0);
		}
	}
	while (argv[index] && argv[index][0] == '-')
		index++;
	if (arg & ARG_S)
		ft_execute();
	else if (arg & ARG_L)
		ft_print_history(arg, index, argv);
	else
		ft_fc();
	return (1);
}