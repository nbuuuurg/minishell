/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 04:16:49 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:24:57 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	len;

	len = 0;
	while (src[len])
		len++;
	if (size == 0)
		return (len);
	while (*src && size - 1 > 0)
	{
		*dest++ = *src++;
		size--;
	}
	*dest = '\0';
	return (len);
}
/*
#include <stdio.h>
int	main(void)
{
	size_t len;
	char dest[10];
	const char src[] = "Bonjour";
	len = ft_strlcpy(dest, src, 10);
	printf("%s\n", dest);
	return (0);
}*/
