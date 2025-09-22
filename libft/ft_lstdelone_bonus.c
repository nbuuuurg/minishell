/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:16:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/04/29 17:01:41 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}

/*
void	ft_del(void *content)
{
	if (content)
		free(content);
}

#include <stdio.h>
int	main(void)
{
	t_list	*lst = NULL;
	char	*s1 = ft_strdup("Bonjour, ");
	char	*s2 = ft_strdup("Je vais bientot disparaitre");
	t_list	*elem1 = ft_lstnew(s1);
	t_list	*elem2 = ft_lstnew(s2);
	
	ft_lstadd_back(&lst, elem1);
	ft_lstadd_back(&lst, elem2);
	printf("%s%s\n", (char *)lst->content, (char *)lst->next->content);
	ft_lstdelone(elem2, ft_del);
	printf("%s\n", (char *)lst->content);
	// no longer exist -> segfault
	printf("%s\n", (char *)lst->next->content);
	return (0);
}*/
