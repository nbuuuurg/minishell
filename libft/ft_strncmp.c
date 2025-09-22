/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 05:11:22 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:22:11 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*c1;
	unsigned char	*c2;

	c1 = (unsigned char *)s1;
	c2 = (unsigned char *)s2;
	while (n > 0)
	{
		if (*c1 != *c2 || *c1 == '\0')
			return ((int)(*c1 - *c2));
		c1++;
		c2++;
		n--;
	}
	return (0);
}

/*
#include <stdio.h>
int	main(void)
{
	const char *s1 = "Bonjour";
	const char *s2 = "Bonjours";
	printf("%d\n", ft_strncmp(s1, s2, 8));
	return (0);
}*/
