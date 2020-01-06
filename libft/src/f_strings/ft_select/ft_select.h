/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 18:06:36 by bprunevi          #+#    #+#             */
/*   Updated: 2020/01/06 17:39:05 by yberramd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <term.h>
# include <unistd.h>
# include <curses.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include "libft.h"

# define SELECTED 1
# define CURSOR 2
# define MAX_ARG_SIZE 1024
# define DISPLAY_REFRESH() sig_handler(SIGWINCH)

t_list *g_list;
int g_max_elem_size;
int g_nb_elems;
int g_nb_lines;
int g_ttyd;

int		biggest_arg_size();
t_list	*argvtolist(char **argv);
int		display(unsigned short x, unsigned short y, t_list *tlist);
void	sig_handler (int sig);
int		set_value(int target, int value);
int		remove_value(int target, t_list *tlist);
int		ttyd_putchar(int c);
int		cursor();
int		scroll_down(unsigned short *y, int nb_lines);
void	final_output(int success, t_list *tlist);
int		get_cursor_pos(unsigned short *x, unsigned short *y);
void	sig_handler(int sig);
#endif
