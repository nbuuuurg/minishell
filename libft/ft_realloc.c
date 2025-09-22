/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:20:05 by nburgevi          #+#    #+#             */
/*   Updated: 2025/07/23 15:33:25 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	char	*p;
	size_t	copy_size;

	p = ft_calloc(1, new_size);
	if (!p)
		return (NULL);
	if (ptr)
	{
		if (new_size > old_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(p, ptr, copy_size);
		free(ptr);
	}
	return ((void *) p);
}
