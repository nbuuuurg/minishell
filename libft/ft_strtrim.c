/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:01:49 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 21:35:46 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	sft_ischarset(const char *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static size_t	sft_newstrlen(const char *set, const char *s1)
{
	size_t	len_s;

	len_s = ft_strlen(s1);
	while (sft_ischarset(set, *(s1 + len_s - 1)) && len_s > 0)
		len_s--;
	while (sft_ischarset(set, *s1) && len_s > 0)
	{
		s1++;
		len_s--;
	}
	return (len_s);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*trim;
	char	*cp;
	size_t	len_s;

	if (!s1 || !set)
		return (NULL);
	len_s = sft_newstrlen(set, s1);
	trim = ft_calloc(len_s + 1, 1);
	if (trim == NULL)
		return (NULL);
	cp = trim;
	while (*s1 || len_s > 0)
	{
		while (sft_ischarset(set, *s1))
			s1++;
		while (len_s > 0)
		{
			*cp++ = *s1++;
			len_s--;
		}
		break ;
	}
	return (trim);
}

/*
int	main(void)
{
	char *s = " \t \n \n   \n\n\n\t";
	char *set = " \n\t";
	char *trim;

	trim = ft_strtrim(s, set);
	printf ("%s\n", trim);
	free(trim);
	return (0);
}*/
