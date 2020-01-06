/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 19:38:57 by bprunevi          #+#    #+#             */
/*   Updated: 2019/06/19 00:45:21 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	interpret_input(char *read_buff, int *target)
{
	if (read_buff[2] == 'A')
		if (set_value((*target) - 1, SELECTED))
			set_value((*target)--, SELECTED);
	if (read_buff[2] == 'B')
		if (set_value((*target) + 1, SELECTED))
			set_value((*target)++, SELECTED);
	if (read_buff[2] == 'D')
		if (set_value((*target) - g_nb_lines, SELECTED))
		{
			set_value((*target), SELECTED);
			*target -= g_nb_lines;
		}
	if (read_buff[2] == 'C')
		if (set_value((*target) + g_nb_lines, SELECTED))
		{
			set_value((*target), SELECTED);
			*target += g_nb_lines;
		}
	if (read_buff[0] == ' ')
	{
		set_value((*target), CURSOR);
		if (set_value((*target) + 1, SELECTED))
			set_value((*target)++, SELECTED);
	}
	else if (read_buff[2] == '3' || read_buff[0] == '\177')
	{
		remove_value((*target), g_list);
		set_value(--(*target), SELECTED);
	}
	else if (read_buff[0] == '\12')
		final_output(1, g_list);
	else if (read_buff[2] == '\0')
		final_output(0, g_list);
	g_nb_lines = 0;
	DISPLAY_REFRESH();
}

int		cursor(void)
{
	char	buffer[6];
	char	*read_buff;
	int		target;

	target = g_nb_elems;
	read_buff = (char *)buffer;
	g_nb_lines = -1;
	DISPLAY_REFRESH();
	ft_bzero(read_buff, 6);
	while (1)
	{
		target = target < g_nb_elems ? target + g_nb_elems : target;
		target = target > g_nb_elems * 2 ? target - g_nb_elems : target;
		if (read(0, read_buff, 6) == -1)
			exit(-1);
		if (ft_strchr(" \33\177\12", read_buff[0]))
			interpret_input(read_buff, &target);
		ft_bzero(read_buff, 6);
	}
	return (0);
}

int		scroll_down(unsigned short *y, int nb_lines)
{
	int			nb;
	static int	check = 0;

	nb = nb_lines;
	if (check)
		return (0);
	check = 0;
	while (--nb > 0)
		write(g_ttyd, "\n", 1);
	nb = nb_lines - (tgetnum("li") - *y);
	if (nb > 0)
		*y -= nb;
	tputs(tgoto(tgetstr("cm", NULL), 0, *y), g_ttyd, ttyd_putchar);
	return (0);
}

int		get_cursor_pos(unsigned short *x, unsigned short *y)
{
	char	buff[16];
	char	*tok1;
	char	*tok2;
	int		i;

	*x = 0;
	*y = 0;
	i = -1;
	ft_bzero(buff, 16);
	ft_strcpy(buff, "\n0[6n");
	buff[1] = 0x1B;
	write(g_ttyd, buff, 5);
	read(g_ttyd, &buff, 16);
	while (++i < 16)
		if (!ft_isdigit(buff[i]))
			buff[i] = ' ';
	tok1 = ft_strtok(buff, " ");
	tok2 = ft_strtok(NULL, " ");
	if (tok1)
		*y = ft_atoi(tok1) - 2;
	if (tok2)
		*x = ft_atoi(tok2) - 1;
	tputs(tgoto(tgetstr("cm", NULL), *x, *y),
			g_ttyd, ttyd_putchar);
	return (0);
}
