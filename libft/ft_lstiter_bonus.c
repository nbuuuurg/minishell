/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:30:05 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 16:47:50 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst && f)
	{
		while (lst)
		{
			f(lst->content);
			lst = lst->next;
		}
	}
}

/*
void	ft_f(void *content)
{
	if (content)
		free(content);
	content = ft_strdup("Lala");
}

int     main(void)
{
	t_list	*lst = NULL;
	char	*s1 = ft_strdup("phrfwef");
	char	*s2 = ft_strdup("kededj");
	t_list  *elem1 = ft_lstnew(s1);
	t_list  *elem2 = ft_lstnew(s2);

	ft_lstadd_back(&lst, elem1);
	ft_lstadd_back(&lst, elem2);
	
	printf("%s%s\n", (char *)lst->content, (char *)lst->next->content);
	ft_lstiter(lst, ft_f);
	printf("%s%s\n", (char *)lst->content, (char *)lst->next->content);
	
	return (0);
}*/
