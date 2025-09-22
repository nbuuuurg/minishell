/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:16:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/04/29 17:01:41 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	n;

	if (!lst)
		return (0);
	n = 0;
	while (lst)
	{
		n++;
		lst = lst->next;
	}
	return (n);
}

/*
#include <stdio.h>
int	main(void)
{
	t_list *lst = NULL;
	t_list *elem1 = ft_lstnew("Bonjour");
	t_list *elem2 = ft_lstnew("Bonjour");
	t_list *elem3 = ft_lstnew("Bonjour");
	t_list *elem4 = ft_lstnew("Bonjour");
	t_list *elem5 = ft_lstnew("Bonjour");
	t_list *elem6 = ft_lstnew("Bonjour");

	ft_lstadd_front(&lst, elem1);
	ft_lstadd_front(&lst, elem2);
	ft_lstadd_front(&lst, elem3);
	ft_lstadd_front(&lst, elem4);
	ft_lstadd_front(&lst, elem5);
	ft_lstadd_front(&lst, elem6);

	printf("%d\n", ft_lstsize(lst));
	free(elem1);
	free(elem2);
	free(elem3);
	free(elem4);
	free(elem5);
	free(elem6);
	
	return (0);
}*/
