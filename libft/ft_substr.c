/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:55:43 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 21:17:04 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*sub;

	i = 0;
	if (start >= ft_strlen(s))
		return (ft_calloc(1, 1));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	while (start > 0)
	{
		i++;
		start--;
	}
	sub = ft_calloc(len + 1, 1);
	if (sub == NULL)
		return (NULL);
	j = 0;
	while (s[i + j] && len > 0)
	{
		sub[j] = s[i + j];
		j++;
		len--;
	}
	return (sub);
}

/*
#include <stdio.h>
int	main(void)
{
	const char *s = "hola";
	char *str;

	str = ft_substr(s, 2, 3);
	printf("%s\n", str);
	return (0);
}*/
