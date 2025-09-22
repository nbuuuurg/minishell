/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:03:29 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/26 17:26:55 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hexlen(unsigned long num)
{
	int	i;

	i = 0;
	while (num > 15)
	{
		i++;
		num /= 16;
	}
	i++;
	return (i);
}

char	*ft_alloc_mem(unsigned long num)
{
	int		len;
	char	*hex;

	len = ft_hexlen(num);
	hex = ft_calloc(len + 1, 1);
	if (hex == NULL)
		return (NULL);
	return (hex);
}

char	*ft_fill_hex(char *hex, char *base, int len, unsigned long n)
{
	if (n == 0)
	{
		hex[0] = '0';
		return (hex);
	}
	while (n != 0)
	{
		hex[len] = base[n % 16];
		n /= 16;
		len--;
	}
	return (hex);
}

char	*ft_itoa_hex(int format, unsigned long num)
{
	unsigned long	n;
	int				i;
	char			*base_16;
	char			*hex;

	hex = ft_alloc_mem(num);
	if (hex == NULL)
		return (NULL);
	if (format == 'x')
		base_16 = "0123456789abcdef";
	else
		base_16 = "0123456789ABCDEF";
	n = num;
	i = ft_hexlen(num) - 1;
	hex = ft_fill_hex(hex, base_16, i, n);
	return (hex);
}

void	ft_printhex(char *hex)
{
	if (hex)
		ft_putstr_fd(hex, 1);
	free(hex);
}
