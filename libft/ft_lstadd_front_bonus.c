/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:01:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 15:11:31 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	if (new)
	{
		if (lst == NULL)
			*lst = new;
		else
		{
			new->next = *lst;
			*lst = new;
		}
	}
}

/*
#include <stdio.h>
int	main(void)
{
	t_list *lst = NULL;
	char	*s1 = "Je suis s1";
	char	*s2 = "Je suis s2";
	char	*s3 = "Je suis s3";
	char	*s4 = "Je suis new";

	t_list *elem1 = ft_lstnew(s3);
	t_list *elem2 = ft_lstnew(s2);
	t_list *elem3 = ft_lstnew(s1);
	t_list *new = ft_lstnew(s4);


	ft_lstadd_front(&lst, elem1);
	ft_lstadd_front(&lst, elem2);
	ft_lstadd_front(&lst, elem3);
	ft_lstadd_front(&lst, new);

	printf("lst[0] = %s\n",(char *)lst->content);
	printf("lst[1] = %s\n",(char *)lst->next->content);
	printf("lst[2] = %s\n",(char *)lst->next->next->content);
	printf("lst[3] = %s\n",(char *)lst->next->next->next->content);

	free(elem1);
	free(elem2);
	free(elem3);
	free(new);
	return (0);
}*/
