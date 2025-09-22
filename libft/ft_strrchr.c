/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 04:49:57 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:21:49 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t		len;
	const char	*p;

	len = 0;
	p = s;
	while (s[len])
		len++;
	while (len + 1 > 0)
	{
		if (*(p + len) == (const char)c)
			return ((char *)(p + len));
		len--;
	}
	return (NULL);
}

/*
#include <stdio.h>
int     main(void)
{
        const char *s = "Bonjour";
        char    c = 'B';
        char *p;

        p = ft_strrchr(s, c);
        printf("%s\n", p);
        return (0);
}*/
