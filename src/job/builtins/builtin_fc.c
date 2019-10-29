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
	unsigned long	nbr;
	int				i;
	int				sign;
	unsigned short	val;

	i = 0;
	nbr = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] > 8 && str[i] < 14))
		++i;
	if (str[i] == '-')
	{
		sign = -1;
		++i;
	}
	while (str[i] > 47 && str[i] < 58)
	{
		val = str[i] ^ ((1 << 5) | (1 << 4));
		nbr = nbr * 10 + val;
		if (nbr > 500)
			return (501);
		++i;
	}
	return (nbr * sign);
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

static void		ft_no_number(int arg, int max)
{
	int		i;
	char	*cmd;

	i = 1;
	max -= 17;
	history(FIRST, NULL, &cmd);
	if (history(GET, NULL, &cmd) && (i - max) > 0 && !(arg & ARG_R))
	{
		if (!(arg & ARG_N))
			ft_printf("%d", i);
		ft_printf("\t%s\n", cmd);
	}
	while (history(FORWARD, NULL, &cmd) != 2 && cmd)
	{
		i++;
		if (!(arg & ARG_N) && (i - max) > 0 && !(arg & ARG_R) && i != (max + 17))
			ft_printf("%d", i);
		if ((i - max) > 0 && !(arg & ARG_R) && i != (max + 17))
			ft_printf("\t%s\n", cmd);
	}
	if (arg & ARG_R)
	{
		while (history(BACKWARD, NULL, &cmd) != 2 && cmd && (i - max) > 0)
		{
			i--;
			if (!(arg & ARG_N) && (i - max) > 0)
				ft_printf("%d", i);
			if ((i - max) > 0)
				ft_printf("\t%s\n", cmd);
		}
	}
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
		if (argv[index + 1] == NULL)// QUAND IL Y A 1 NOMBRE
		{
			nbr = ft_atoi_history(argv[index]);
			if (!nbr) // S'IL EST EGAL A 0
				nbr = 500;
			else if (nbr > 0)// S'IL est SUPERIEUR A 0
			{
				history(FIRST, NULL, &cmd);
				if (!(arg & ARG_R) && i != max && history(GET, NULL, &cmd) && ((i - nbr) >= 0 || (i == max - 1)))
				{
					if (!(arg & ARG_N))
						ft_printf("%d", i);
					ft_printf("\t%s\n", cmd);
				}
				while (history(FORWARD, NULL, &cmd) != 2 && cmd)
				{
					i++;
					if (i != max && !(arg & ARG_N) && ((i - nbr) >= 0 || i == (max - 1)) && !(arg | ARG_R))
						ft_printf("%d", i);
					if (i != max && ((i - nbr) >= 0 || (i == max - 1)) && !(arg | ARG_R))
						ft_printf("\t%s\n", cmd);
				}
				if (arg | ARG_R)// SI ARG R EST UTILISER
				{
					while (history(BACKWARD, NULL, &cmd) != 2 && cmd)
					{
						i--;
						if (!(arg & ARG_N) && ((i - nbr) >= 0 || i == (max - 1)))
							ft_printf("%d", i);
						if ((i - nbr) >= 0 || (i == max - 1))
							ft_printf("\t%s\n", cmd);
					}
				}
			}
			else if (nbr < 0)// S'IL EST NEGATIF
				ft_printf("NEGATIF\n");
		}
		else// S'IL IL Y A DEUX NBR
		{
			ft_printf("COUCOU\n");
		}
	}
	else
		ft_no_number(arg, max);
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
	while ((opt = getopt(argc, argv, "rnlse:0123456789")) != -1)// A REMPLACER PAR FT_GETOPT DE ANTOINE/**/
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
		else if (opt >= 48 && opt <= 57)/*[e]*/
			arg = arg | ARG_NUMBER;
		else
		{
			ft_printf("fc: usage: fc [-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]\n");
			return (0);
		}
	}
	while (argv[index] && argv[index][0] == '-' && !ft_isdigit(argv[index][1]))
		index++;
	if (arg & ARG_S)
		ft_execute();
	else if (arg & ARG_L)
		ft_print_history(arg, index, argv);
	else
		ft_fc();
	return (1);
}