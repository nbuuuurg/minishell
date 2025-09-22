/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:01:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 15:12:05 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*
#include <stdio.h>
int     main(void)
{
	t_list *lst = NULL;
	char    *s1 = "Je suis s1";
	char    *s2 = "Je suis s2";
	char    *s3 = "Je suis s3";
	char    *last = "Je suis le last";

	t_list *elem1 = ft_lstnew(s3);
	t_list *elem2 = ft_lstnew(s2);
	t_list *elem3 = ft_lstnew(s1);
	t_list *elem4 = ft_lstnew(last);


	ft_lstadd_front(&lst, elem4);
	ft_lstadd_front(&lst, elem1);
	ft_lstadd_front(&lst, elem2);
	ft_lstadd_front(&lst, elem3);
	
	printf("%s\n",(char *)(ft_lstlast(lst)->content));

	free(elem1);
	free(elem2);
	free(elem3);
	free(elem4);
	return (0);
}*/
