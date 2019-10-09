/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberramd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/14 13:53:47 by yberramd          #+#    #+#             */
/*   Updated: 2019/10/08 16:55:03 by yberramd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "history.h"

static int	delete_history(t_history *history)
{
	t_history	*tmp;

	if (history->str)
		ft_strdel(&history->str);
	tmp = history->next;
	history = NULL;
	history = tmp;
	while (history)
	{
		if (history->str)
			ft_strdel(&history->str);
		tmp = history->next;
		free(history);
		history = NULL;
		history = tmp;
	}
	return (1);
}

static int	delete(t_history *history, char *home)
{
	ft_strdel(&home);
	delete_history(history);
	return (1);
}

static int	w_history(char *line, int fd)
{
	int	len;

	len = ft_strlen(line);
	if (write(fd, line, len) != len)
		return (-1);
	if (write(fd, "\n", 1) != 1)
		return (-1);
	return (1);
}

static int	add_history(char *line, t_history *history)
{
	if (history->str)
		{
			if (!(history->next = (t_history*)malloc(sizeof(t_history))))
				return (-1);
			history->next->previous = history;
			history = history->next;
			history->next = NULL;
			if (!(history->str = ft_strdup(line)))
				return (-1);
			}
	else if (!(history->str = ft_strdup(line)))
		{
			history->next = NULL;
			return (-1);
		}
	return (1);
}

/*static int	ft_strcmp_n(const char *line, const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while ((line[i] == str[i]) && (line[i] || str[i] != '\0'))
		i++;
	if (line[i] == '\0' && str[i] == '\n')
		return (0);
	return (1);
}*/

static int	add_cmd(char *line, t_history *history, char *home)
{
	int	fd;

	fd = open(home, O_WRONLY | O_APPEND);
	if (fd != -1)
	{
		while (history->next)
			history = history->next;
		if ((history->str == NULL || ft_strcmp(line, history->str) != 0)
			 && ft_isspace(line[0]) != 1 && line[0] != '\0')
		{
			if (add_history(line, history) == -1)
			{
				close(fd);
				return (-1);
			}
			if (w_history(line, fd) == -1)
			{
				close(fd);
				return (-1);
			}
		}
		close(fd);
		return (1);
	}
	return (-1);
}

static int	ft_search(t_history *history, char *line, char **cmd)
{
	while (history->next)
		history = history->next;
	while (history->previous)
	{
		if (history->str != NULL && line != NULL && ft_strstr(history->str, line) != NULL)
		{
			*cmd = history->str;
			return (1);
		}
		history = history->previous;
	}
	return (-1);
}

static int	search_history(t_history *history, char *line, char **cmd)
{
	while (history->next)
		history = history->next;
	while (history->previous)
	{
		if (history->str != NULL && line != NULL && ft_strcmp(history->str, line) > 0)
		{
			*cmd = history->str;
			return (1);
		}
		history = history->previous;
	}
	return (-1);
}

static int init_clean(int fd, char *line)
{
	close(fd);
	ft_strdel(&line);
	return (-1);
}

static int	init_history(t_history *history, char **home)
{
	int		fd;
	char	*line;

	line = NULL;
	if (!(*home = ft_strjoin(getenv("HOME"), "/.42sh_history")))
		return (-1);
	if ((fd = open(*home, O_RDONLY | O_CREAT, 0600)) != -1)
	{
		while (get_next_line(fd, &line) > 0)
		{
			if (!(history->str = ft_strdup(line)))
				return (init_clean(fd, line));
			if (!(history->next = (t_history*)malloc(sizeof(t_history))))
				return (init_clean(fd, line));
			history->next->previous = history;
			history = history->next;
			ft_strdel(&line);
		}
		if (history->previous)
		{
			history = history->previous;
			free(history->next);
			history->next = NULL;
		}
		init_clean(fd, line);
		return (1);
	}
	return (-1);
}

static int		ft_atoi_history(const char *str)
{
	unsigned long	nbr;
	int				i;
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
			return (0);
		++i;
	}
	return (nbr);
}

static int		exclamation_point_number(char *line, t_history *history, char **cmd)
{
	int nbr;

	if ((nbr = ft_atoi_history(&line[1])) <= 0)
		return (0);
	while (history->next && nbr > 1)
	{
		history = history->next;
		nbr--;
	}
	if (nbr > 1)
		return (0);
	if (history)
		*cmd = history->str;
	return (1);
}

static int		exclamation_point_minus_number(char *line, t_history *history, char **cmd)
{
	int nbr;

	while (history->next)
		history = history->next;
	if ((nbr = ft_atoi_history(&line[2])) <= 0)
		return (0);
	while (history->previous && nbr > 1)
	{
		history = history->previous;
		nbr--;
	}
	if (nbr > 1)
		return (0);
	if (history)
		*cmd = history->str;
	return (1);
}

static int		exclamation_point_exclamation_point(t_history *history, char **cmd)
{
	while (history->next)
		history = history->next;
	if (history)
		*cmd = history->str;
	return (1);
}

static int		exclamation_point(char *line, t_history *history, char **cmd, char *home)
{
	int ret;


	if (ft_isdigit(line[1]))
		ret = exclamation_point_number(line, history, cmd);
	if (line[1] == '-' && ft_isdigit(line[2]))
		ret = exclamation_point_minus_number(line, history, cmd);
	if (line[1] == '!')
		ret = exclamation_point_exclamation_point(history, cmd);
	if (line[1] != '\0')
		ret = search_history(history, &line[1], cmd);
	if (ret)
		add_cmd(*cmd, history, home);//A VOIR C'eST MOCHE
	return (ret);
}

int 	print_history(t_history *history)
{
	int i;

	i = 1;
	if (history)
	{
		while (history->next)
		{
			ft_printf("%d\t", i);
			ft_printf("%s\n", history->str);
			history = history->next;
			i++;
		}
	}
	return (1);
}

int		history(int flag, char *line, char **cmd)//LIMIT HISTORY POUR LE RENDRE POSIX
{
	static t_history	history = {NULL, NULL, NULL};
	static char			*home = NULL;

	/*if (flag == FC)
		return (fc(&history));*/
	if (flag == PRINT_HISTORY)
		return (print_history(&history));	
	if (flag == EXCLAMATION && line[0] == '!')
		return (exclamation_point(line, &history, cmd, home));//ajout de ADD_CMD
	if (flag == INIT)
		return (init_history(&history, &home));
	if (flag == DELETE)
		return (delete(&history, home));
	if (flag == ADD_CMD)
		return (add_cmd(line, &history, home));
	if (flag == SEARCH)
		return (ft_search(&history, line, cmd));//from anywhere
	if (flag == HISTORY_SEARCH)
		return (search_history(&history, line, cmd));//debut fin
	return (-1);
}
