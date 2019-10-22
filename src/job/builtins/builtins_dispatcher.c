/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatcher.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthel <abarthel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:52:32 by abarthel          #+#    #+#             */
/*   Updated: 2019/10/16 19:57:49 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "builtins.h"
#include "error.h"

const t_builtins	g_builtins[] =
{
	{ "echo", &cmd_echo},
	{ "hash", &cmd_hash},
	{ "exit", &cmd_exit},
	{ "env", &cmd_env},
	{ "setenv", &cmd_setenv},
	{ "unsetenv", &cmd_unsetenv},
	{ "pwd", &cmd_pwd},
	{ "cd", &cmd_cd},
	{ "true", &cmd_true},
	{ "false", &cmd_false},
	{ "type", &cmd_type},
	{ "history", &cmd_history},
	{ "fc", &cmd_fc},
	{ "\0", NULL}
};

_Bool	prior_builtin(char *str)
{
	const struct s_prior_builtin	pbuiltin_list[] =
	{ {"echo"}, {"exit"}, {"hash"}, {"setenv"}, {"unsetenv"}, {"pwd"}, {"cd"}, {"type"}, {"fc"}, {"\0"} };
	int	i;

	i = 0;
	while (*(pbuiltin_list[i].pbuiltin))
	{
		if (!ft_strcmp(str, pbuiltin_list[i].pbuiltin))
			return (1);
		++i;
	}
	return (0);
}

static void	*dispatcher(char *cmd)
{
	int	i;

	i = 0;
	while (g_builtins[i].key[0] && ft_strcmp(cmd, g_builtins[i].key))
		++i;
	if (!(g_builtins[i].key[0]))
		return (NULL);
	else
		return ((void*)g_builtins[i].f);
}

int		is_a_builtin(char *cmd)
{
	if (dispatcher(cmd))
		return (1);
	else
		return (0);
}

int			builtins_dispatcher(char **argv)
{
	int			(*f)(int, char**);
	int			ret;
	int			argc;

	argc = 0;
	ret = e_command_not_found;
	if ((f = dispatcher(*argv)))
	{
		while (argv[argc])
			++argc;
		ret = f(argc, argv);
	}
	return (ret);
}
