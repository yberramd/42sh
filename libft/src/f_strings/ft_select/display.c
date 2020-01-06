/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 19:40:50 by bprunevi          #+#    #+#             */
/*   Updated: 2019/06/19 00:43:51 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		display(unsigned short x, unsigned short y, t_list *tlist)
{
	char	buffer[MAX_ARG_SIZE + 8];
	char	*buff;
	int		i;

	buff = (char *)&buffer;
	tputs(tgetstr("me", NULL), g_ttyd, ttyd_putchar);
	tputs(tgetstr("cd", NULL), g_ttyd, ttyd_putchar);
	i = 0;
	while (tlist != g_list || !i)
	{
		buff = ft_strcpy(buff, tgoto(tgetstr("cm", NULL),
		(i / g_nb_lines) * (g_max_elem_size) + x, i % g_nb_lines + y));
		buff = ft_strcat(buff,
		tgetstr(tlist->content_size & SELECTED ? "mr" : "me", NULL));
		buff = ft_strcat(buff,
		tgetstr(tlist->content_size & CURSOR ? "us" : "ue", NULL));
		buff = ft_strcat(buff, tlist->content);
		tlist = tlist->next;
		tputs(buff, g_ttyd, ttyd_putchar);
		i++;
	}
	tputs(tgetstr("me", NULL), g_ttyd, ttyd_putchar);
	tputs(tgoto(tgetstr("cm", NULL), x, y), g_ttyd, ttyd_putchar);
	return (0);
}
