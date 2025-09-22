/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:49:22 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 21:53:01 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_elem;
	void	*content;

	new_lst = NULL;
	if (!lst || !del || !f)
		return (NULL);
	while (lst != NULL)
	{
		content = f(lst->content);
		new_elem = ft_lstnew(content);
		if (new_elem == NULL)
		{
			del(content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_elem);
		lst = lst->next;
	}
	return (new_lst);
}

/*void	ft_del(void *content)
{
	if (content)
		free(content);
}

void    *ft_f(void *content)
{
	if (content)
		free(content);
        content = ft_strdup("Lala");
	return (content);
}

#include <stdio.h>
int     main(void)
{
        t_list  *lst = NULL;
	t_list	*new_lst;
        char    *s1 = ft_strdup("phrfwef");
        char    *s2 = ft_strdup("kededj");
        t_list  *elem1 = ft_lstnew(s1);
        t_list  *elem2 = ft_lstnew(s2);

        ft_lstadd_back(&lst, elem1);
        ft_lstadd_back(&lst, elem2);  
	new_lst = ft_lstmap(lst, ft_f, ft_del);
        printf("%s%s\n", (char *)new_lst->content, 
(char *)new_lst->next->content);
	ft_lstclear(&lst, ft_del);
	ft_lstclear(&new_lst, ft_del);
        return (0);
}*/
