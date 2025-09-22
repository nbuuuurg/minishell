/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:01:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 19:40:59 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	unsigned char	i;
	char			*str;
	size_t			len;

	i = 0;
	len = ft_strlen(s);
	str = ft_calloc(len + 1, 1);
	if (str == NULL)
		return (NULL);
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	return (str);
}

/*
char	ft_f(unsigned int i, char c)
{
	return (c + i);
}

#include <stdio.h>
int	main(void)
{
	char *str;
	const char *s = "0123456";

	str = ft_strmapi(s, ft_f);
	printf("%s\n", str);
	return (0);
}*/
