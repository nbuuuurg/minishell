/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:01:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 21:25:12 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*sft_swap(char *str, int len)
{
	int		i;
	char	temp;

	i = 0;
	if (str[i] == '-')
		i++;
	while (i < len)
	{
		temp = str[i];
		str[i] = str[len];
		str[len] = temp;
		i++;
		len--;
	}
	return (str);
}

static int	sft_itoalen(int n)
{
	int	len;

	len = 1;
	if (n == 0)
		return (len);
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n > 9)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static char	*sft_convert(char *num, int n)
{
	char	*r;

	r = num;
	if (n < 0)
	{
		*r++ = '-';
		n *= -1;
	}
	if (n < 10)
		*r = n + '0';
	while (n > 9)
	{
		*r++ = n % 10 + '0';
		n = n / 10;
	}
	*r = n % 10 + '0';
	return (num);
}

char	*ft_itoa(int n)
{
	char	*num;
	int		len;

	num = NULL;
	if (n == -2147483648)
	{
		num = ft_strdup("-2147483648");
		return (num);
	}
	len = sft_itoalen(n);
	num = ft_calloc(len + 1, 1);
	if (num == NULL)
		return (NULL);
	num = sft_convert(num, n);
	return (sft_swap(num, len - 1));
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%s\n", ft_itoa(-2147483648));
	return (0);
}*/
