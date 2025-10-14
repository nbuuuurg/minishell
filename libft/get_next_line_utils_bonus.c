/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:03:59 by nburgevi          #+#    #+#             */
/*   Updated: 2025/06/24 16:35:32 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_gnl(char *save, char *buff)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (!save)
		save = ft_calloc(1, 1);
	if (!save)
		return (ft_free_static(save), save = NULL);
	str = ft_calloc(ft_strlen(save) + ft_strlen(buff) + 1, 1);
	if (str == NULL)
		return (ft_free_static(save), save = NULL);
	i = 0;
	j = 0;
	while (save[i])
	{
		str[i] = save[i];
		i++;
	}
	while (buff[j])
		str[i++] = buff[j++];
	if (save)
		free(save);
	save = 0;
	return (str);
}

char	*ft_save(char *save)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == 0)
		return (save[0] = 0, save);
	str = ft_calloc(ft_strlen(save) - i + 1, 1);
	if (!str)
		return (ft_free_static(save), save = NULL);
	i++;
	j = 0;
	while (save[i])
		str[j++] = save[i++];
	if (save)
		free(save);
	save = 0;
	return (str);
}
