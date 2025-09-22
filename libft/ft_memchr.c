/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:22:31 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:10:29 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n--)
	{
		if (*p == (unsigned char)c)
			return ((void *)p);
		p++;
	}
	return (NULL);
}

/*
#include <stdio.h>
int	main(void)
{
	char	d[] = "Bonjour 42";
	char	c = 'o';
	void	*p;

	p = ft_memchr(d, c, 10);
	printf ("%s\n", (char *)p);
	return (0);
}*/
