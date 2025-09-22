/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 04:41:16 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:09:21 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	const char	*p;

	p = s;
	while (*p)
	{
		if (*p == (const char)c)
			return ((char *)p);
		p++;
	}
	if (*p == (const char)c)
		return ((char *)p);
	return (NULL);
}

/*
#include <stdio.h>
int	main(void)
{
	const char *s = "Bonjour";
	char	c = 'o';
	char *p;

	p = ft_strchr(s, c);
	printf("%s\n", p);
	return (0);
}*/
