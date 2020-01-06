/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/16 01:09:30 by bprunevi          #+#    #+#             */
/*   Updated: 2019/06/19 00:44:52 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	sigwinch(void)
{
	int						nb_tcols;
	int						new_nb_lines;
	static unsigned short	x;
	static unsigned short	y;

	tgetent(NULL, getenv("TERM"));
	nb_tcols = tgetnum("co");
	if (g_max_elem_size <= nb_tcols)
		new_nb_lines = (g_nb_elems) / (nb_tcols / g_max_elem_size)
			+ !!(g_nb_elems % (nb_tcols / g_max_elem_size));
	else
		new_nb_lines = g_nb_elems;
	if (g_nb_lines != 0)
	{
		get_cursor_pos(&x, &y);
		scroll_down(&y, new_nb_lines);
	}
	if (new_nb_lines > tgetnum("li"))
	{
		tputs(tgetstr("cl", NULL), g_ttyd, ttyd_putchar);
		if (nb_tcols > 37)
			write(g_ttyd, "Waiting for window to be big enough...", 38);
	}
	else if (g_nb_lines != new_nb_lines && (g_nb_lines = new_nb_lines))
		display(x, y, g_list);
}

void	sig_handler(int sig)
{
	struct termios			term;

	if (sig == SIGWINCH)
		sigwinch();
	if (sig == SIGINT)
		final_output(0, g_list);
	if (sig == SIGTSTP)
	{
		tputs(tgetstr("cd", NULL), g_ttyd, ttyd_putchar);
		tputs(tgetstr("ve", NULL), g_ttyd, ttyd_putchar);
		signal(SIGCONT, sig_handler);
		signal(SIGTSTP, SIG_DFL);
		ioctl(0, TIOCSTI, "\032");
	}
	if (sig == SIGCONT)
	{
		ft_bzero(&term, sizeof(struct termios));
		tcgetattr(0, &term);
		term.c_lflag = ISIG;
		tcsetattr(0, 0, &term);
		g_nb_lines = -1;
		tputs(tgetstr("vi", NULL), g_ttyd, ttyd_putchar);
		DISPLAY_REFRESH();
		signal(SIGTSTP, sig_handler);
	}
}
