/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:00:26 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 20:25:52 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	size_t	i;
	char	*p;

	total = nmemb * size;
	if (size == 0 || nmemb == 0)
		return (malloc(0));
	if (total / nmemb != size)
		return (NULL);
	p = malloc(total);
	if (p == NULL)
		return (NULL);
	i = 0;
	while (i < total)
	{
		p[i] = '\0';
		i++;
	}
	return ((void *)p);
}

/*
#include <stdio.h>
int	main(void)
{
	char *s;
	size_t i = 0;

	s = ft_calloc(10, 1);
	while (i < 10)
	{
		printf("%c", s[i]);
		i++;
	}
	printf("\n");
	return (0);
}*/
