/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:18:51 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 22:26:44 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *lil, size_t len)
{
	size_t	i;
	size_t	j;
	char	*find;

	find = (char *)big;
	if (ft_strlen(lil) == 0)
		return (find);
	i = 0;
	while (i++ < len && *find)
	{
		j = 0;
		while (*(find + j) == *(lil + j))
		{
			if (*(lil + j + 1) == 0 && i + j <= len)
				return (find);
			j++;
		}
		find++;
	}
	return (NULL);
}

/*
#include <stdio.h>
int	main(void)
{
	const char *big = "lorem ipsum dolor sit amet";
	const char *lil = "dolor";
	char *find;
	find = ft_strnstr(big, lil, ft_strlen(lil));
	printf("%s\n", find);
	return (0);
}*/
