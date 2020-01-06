/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 17:17:35 by bprunevi          #+#    #+#             */
/*   Updated: 2020/01/06 17:12:24 by yberramd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		ttyd_putchar(int c)
{
	write(g_ttyd, &c, 1);
	return (0);
}

int		biggest_arg_size(void)
{
	int		i;
	int		j;
	int		max;
	t_list	*tlist;

	i = 0;
	max = 0;
	tlist = g_list;
	while ((tlist != g_list || !max) && (j = ft_strlen(tlist->content)))
	{
		max = j > max ? j : max;
		tlist = tlist->next;
	}
	if (max > MAX_ARG_SIZE)
		exit(ft_printf("One of the arguments is beyond %d characters !\n",
		MAX_ARG_SIZE) && 0);
	return (max + 1);
}

t_list	*argvtolist(char **argv)
{
	int		i;
	t_list	*list;

	i = 1;
	if (!argv)
		exit(write(g_ttyd, "usage : ft_select [ARGUMENTS ...]\n", 34) && 0);
	list = ft_lstnew(argv[1], 1);
	while (argv[++i])
		ft_lstaddlast(&list, ft_lstnew(argv[i], 0));
	ft_lstaddlast(&list, list);
	return (list);
}

void	final_output(int success, t_list *tlist)
{
	t_list	*previous;
	int		i;

	i = 0;
	tputs(tgetstr("cd", NULL), g_ttyd, ttyd_putchar);
	tputs(tgetstr("ve", NULL), g_ttyd, ttyd_putchar);
	while ((tlist && tlist != g_list) || !i)
	{
		i = i | 1;
		if (tlist->content_size & CURSOR)
		{
			if (success)
			{
				ft_printf(i & 2 ? " %s" : "%s", tlist->content);
				i = i | 2;
			}
		}
		previous = tlist;
		tlist = tlist->next;
		free(previous->content);
		free(previous);
	}
	if (i & 2)
		write(1, "\n", 1);
	exit(!success);
}

int		ft_select(int argc, char **argv)
{
	struct termios term;

	g_nb_elems = argc;
	g_nb_lines = -1;
	g_ttyd = open("/dev/tty", O_RDWR);
	if (signal(SIGWINCH, sig_handler)
		|| signal(SIGINT, sig_handler)
		|| signal(SIGTSTP, sig_handler)
		|| signal(SIGCONT, sig_handler))
		exit(write(g_ttyd, "ft_select : signal() failed\n", 28) && 0);
	g_list = argvtolist(--g_nb_elems > 1 ? argv : NULL);
	g_max_elem_size = biggest_arg_size();
	tgetent(NULL, getenv("TERM"));
	tputs(tgetstr("vi", NULL), g_ttyd, ttyd_putchar);
	tcgetattr(0, &term);
	term.c_lflag = ISIG;
	tcsetattr(0, 0, &term);
	DISPLAY_REFRESH();
	cursor();
	return (0);
}
