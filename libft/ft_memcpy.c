/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 02:44:56 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:24:37 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	while (n--)
		*d++ = *s++;
	return ((void *)dest);
}

/*
#include <stdio.h>
int	main(void)
{
	char dest[10] = "0123456789";
	const char src[10] = "9876543210";
	void *d;

	d = ft_memcpy((void *)dest, (const void *)src, 10);
	printf("%s\n", (char *)d);
	return (0);
}*/
