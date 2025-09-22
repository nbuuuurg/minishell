/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:00:57 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/26 17:26:30 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_format(char c)
{
	if (c == 'c' || c == 'd' || c == 'i')
		return (1);
	if (c == 'u' || c == 'x' || c == 'X')
		return (2);
	if (c == 's')
		return (3);
	if (c == 'p')
		return (4);
	if (c == '%')
		return (5);
	return (0);
}

int	ft_check_format(char format, int total, va_list args)
{
	if (ft_format(format) == 1)
		total += print_int(format, va_arg(args, int));
	if (ft_format(format) == 2)
		total += print_uint(format, va_arg(args, unsigned int));
	if (ft_format(format) == 3)
		total += print_str(va_arg(args, char *));
	if (ft_format(format) == 4)
		total += print_address(va_arg(args, void *));
	if (ft_format(format) == 5)
	{
		ft_putchar_fd(format, 1);
		total++;
	}
	return (total);
}

int	ft_printf(const char *format, ...)
{
	int		total;
	int		i;
	va_list	args;

	va_start(args, format);
	total = 0;
	i = 0;
	if (!format)
		return (-1);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			total = ft_check_format(format[i], total, args);
		}
		else
		{
			ft_putchar_fd(format[i], 1);
			total++;
		}
		i++;
	}
	va_end(args);
	return (total);
}

/*
#include <stdio.h>
int	main(int ac, char **av)
{
	(void) ac;
	char	*str = av[1];
	int i;
	int j;
	ft_printf("mine : ");
	i = ft_printf("%s", str);
	printf(" | ft_printf = %d\n", i);
	printf("real : ");
	j = printf("%s", str);
	printf(" | prinft = %d\n", j);
	return (0);
}*/
