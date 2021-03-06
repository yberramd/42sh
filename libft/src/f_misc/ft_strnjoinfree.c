/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthel <abarthel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 16:21:55 by abarthel          #+#    #+#             */
/*   Updated: 2019/09/29 09:18:59 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include "libft.h"

static char	*concat(char *s1, char *s2)
{
	char *str;

	str = NULL;
	if ((str = ft_strjoinfree(s1, s2)))
	{
		return (str);
	}
	return (NULL);
}

static char	*joinall(int nb, char *s1, char *s2, va_list ap)
{
	char	*final;
	char	*va;

	if (!(final = ft_strjoinfree(s1, s2)))
	{
		return (NULL);
	}
	nb -= 2;
	while (nb)
	{
		va = va_arg(ap, char*);
		final = concat(final, va);
		--nb;
	}
	return (final);
}

char		*ft_strnjoinfree(int nb, char *s1, char *s2, ...)
{
	va_list	ap;
	char	*str;

	str = NULL;
	if (nb == 2)
	{
		if ((str = ft_strjoinfree(s1, s2)))
		{
			return (str);
		}
	}
	else if (nb > 2)
	{
		va_start(ap, s2);
		if ((str = joinall(nb, s1, s2, ap)))
		{
			va_end(ap);
			return (str);
		}
		va_end(ap);
	}
	return (NULL);
}
