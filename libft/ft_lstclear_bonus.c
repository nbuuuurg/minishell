/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:03:42 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 20:51:50 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}

/*
void    ft_del(void *content)
{
	if (content)
		free(content);
}

#include <stdio.h>
int     main(void)
{
	t_list  *lst = NULL;
	char    *s1 = ft_strdup("Bonjour, ");
	char    *s2 = ft_strdup("Je vais bientot disparaitre");
	t_list  *elem1 = ft_lstnew(s1);
	t_list  *elem2 = ft_lstnew(s2);

	ft_lstadd_back(&lst, elem1);
	ft_lstadd_back(&lst, elem2);
	printf("%s%s\n", (char *)lst->content, (char *)lst->next->content);
	ft_lstclear(&lst, ft_del);
	// no longer exist -> segfault
	printf("%s\n", (char *)lst->content);
	printf("%s\n", (char *)lst->next->content);
	return (0);
}*/
