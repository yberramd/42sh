/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 19:40:00 by bprunevi          #+#    #+#             */
/*   Updated: 2019/06/19 00:44:29 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		remove_value(int target, t_list *tlist)
{
	int		i;
	t_list	*tmp;

	i = 0;
	if (g_list->next == g_list)
		final_output(1, g_list);
	if (target % g_nb_elems == 0)
		g_list = g_list->next;
	while (tlist)
	{
		if ((i++ == target - 1 && tlist->next && g_nb_elems--))
		{
			free(tlist->next->content);
			tmp = tlist->next->next;
			free(tlist->next);
			tlist->next = tmp;
			g_max_elem_size = biggest_arg_size();
			return (1);
		}
		tlist = tlist->next;
	}
	return (0);
}

int		set_value(int target, int value)
{
	int		i;
	t_list	*tlist;

	tlist = g_list;
	i = 0;
	while (tlist)
	{
		if (i++ == target)
		{
			tlist->content_size = tlist->content_size ^ value;
			return (1);
		}
		tlist = tlist->next;
	}
	return (0);
}
