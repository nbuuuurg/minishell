/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 02:35:41 by nburgevi          #+#    #+#             */
/*   Updated: 2025/04/27 02:43:48 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	s = ft_memset(s, '\0', n);
}

/*
#include <stdio.h>
int     main(void)
{
        char p[] = "lalalala";

       	ft_bzero((void *)p, 4);
        int	i = 0;
	while (i < 8)
	{
		printf("%c\n", p[i]);
		i++;
	}
        return (0);
}*/
