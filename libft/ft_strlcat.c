/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 04:34:21 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:09:04 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dsize)
{
	size_t	s_len;
	size_t	d_len;
	size_t	i;

	s_len = ft_strlen(src);
	d_len = 0;
	while (dest[d_len] && d_len < dsize)
		d_len++;
	if (dsize <= d_len)
		return (s_len + dsize);
	i = 0;
	while (src[i] && (d_len + i + 1) < dsize)
	{
		dest[d_len + i] = src[i];
		i++;
	}
	dest[d_len + i] = '\0';
	return (s_len + d_len);
}

/*
#include <stdio.h>
int     main(void)
{
        size_t len;
        char dest[10] = "dd";
        const char src[] = "Bonjour";
        len = ft_strlcat(dest, src, 10);
        printf("%s\n", dest);
        return (0);
}*/
