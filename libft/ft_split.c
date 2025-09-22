/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:01:58 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/02 22:03:58 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	sft_free_list(char **str)
{
	char	**temp;

	temp = str;
	while (*temp)
	{
		free(*temp);
		temp++;
	}
	free(str);
}

static size_t	sft_count_words(const char *s, char c)
{
	size_t	word;

	word = 1;
	while (*s && *s == c)
		s++;
	if (*s == 0)
		return (0);
	while (*(s + 1))
	{
		if (*s == c && *(s + 1) != 0 && *(s + 1) != c)
			word++;
		s++;
	}
	return (word);
}

static char	**sft_fill_word(const char *s, char **str, char c, size_t word)
{
	char	**split_str;
	char	*dest;
	size_t	len;

	split_str = str;
	while (word-- > 0)
	{
		while (*s == c)
			s++;
		len = 0;
		while (*(s + len) && *(s + len) != c)
			len++;
		*str = ft_calloc(len + 1, 1);
		if (*str == NULL)
		{
			sft_free_list(split_str);
			return (NULL);
		}
		dest = *str;
		while (*s != c && *s)
			*dest++ = *s++;
		str++;
	}
	*str = NULL;
	return (split_str);
}

char	**ft_split(const char *s, char c)
{
	char	**str;
	char	**split_str;
	size_t	word;

	if (!s)
		return (NULL);
	word = sft_count_words(s, c);
	str = ft_calloc(word + 1, sizeof(char *));
	if (str == NULL)
		return (NULL);
	split_str = sft_fill_word(s, str, c, word);
	return (split_str);
}

/*
#include <stdio.h>
int	main(void)
{
	size_t word;
	size_t i;
	char **split;
	const char *str = "\0aa\0bbb";
	char c = '\0';

	word = sft_count_words(str, c);
	split = ft_split(str, c);
	i = 0;
	while (i < word)
	{
		printf("%s\n", split[i]);
		i++;
	}
	printf("%zu\n", word);
	printf("%s\n", split[i]);
	return (0);
}*/
