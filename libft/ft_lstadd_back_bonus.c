/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:01:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 15:28:13 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_lstlast(*lst);
		temp->next = new;
	}
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

	t_list *elem1 = ft_lstnew(s1);
	t_list *elem2 = ft_lstnew(s2);
	t_list *elem3 = ft_lstnew(s3);
	t_list *elem4 = ft_lstnew(last);


	ft_lstadd_back(&lst, elem1);
	ft_lstadd_back(&lst, elem2);
	ft_lstadd_back(&lst, elem3);
	ft_lstadd_back(&lst, elem4);

	printf("lst[0] = %s\n",(char *)lst->content);
	printf("lst[1] = %s\n",(char *)lst->next->content);
	printf("lst[2] = %s\n",(char *)lst->next->next->content);
	printf("lst[3] = %s\n",(char *)lst->next->next->next->content);

	free(elem1);
	free(elem2);
	free(elem3);
	free(elem4);
	return (0);
}*/
