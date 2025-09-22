/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:05:03 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/26 17:27:06 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_int(int format, int arg)
{
	int	r;

	r = 1;
	if (format == 'c')
		ft_putchar_fd((char)arg, 1);
	else
	{
		ft_putnbr_fd(arg, 1);
		if (arg < 0)
		{
			if (arg == INT_MIN)
				arg++;
			arg = -arg;
			r++;
		}
		while (arg > 9)
		{
			arg = arg / 10;
			r++;
		}
	}
	return (r);
}
