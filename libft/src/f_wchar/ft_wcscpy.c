/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wcscpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthel <abarthel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:05:09 by abarthel          #+#    #+#             */
/*   Updated: 2019/03/05 12:10:12 by abarthel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

wchar_t	*ft_wcscpy(wchar_t *ws1, const wchar_t *ws2)
{
	size_t	i;

	i = 0;
	while (ws2[i])
	{
		ws1[i] = ws2[i];
		++i;
	}
	ws1[i] = '\0';
	return (ws1);
}
