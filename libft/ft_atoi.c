/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:19:56 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 21:38:53 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	sft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	res;

	sign = 1;
	res = 0;
	while (sft_isspace(*nptr))
		nptr++;
	if ((*nptr == '+' || *nptr == '-'))
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		res = res * 10 + *nptr - 48;
		nptr++;
	}
	return (sign * res);
}

// #include <stdio.h>
// int	main(void)
// {
// char *str = "512312>";
// printf("ft_atoi = %d\n", ft_atoi(str));
// printf("atoi = %d\n", atoi(str));
// return (0);
// }
