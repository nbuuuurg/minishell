/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:59:35 by nburgevi          #+#    #+#             */
/*   Updated: 2025/06/24 15:42:13 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_static(char *s)
{
	if (s)
		free(s);
}

char	*ft_get_line(char *save)
{
	int		i;
	char	*str;
	int		j;

	i = 0;
	if (save[0] == '\0')
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == '\n')
		i++;
	str = ft_calloc(i + 1, 1);
	if (!str)
		return (NULL);
	j = 0;
	while (j < i)
	{
		str[j] = save[j];
		j++;
	}
	return (str);
}

char	*ft_read_and_save(int fd, char *save)
{
	char	*buff;
	int		read_bytes;

	buff = ft_calloc(BUFFER_SIZE + 1, 1);
	if (!buff)
		return (ft_free_static(save), save = NULL);
	read_bytes = 1;
	while (ft_strchr(save, '\n') == NULL && read_bytes != 0)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			ft_free_static(save);
			return (free(buff), save = NULL);
		}
		if (read_bytes == 0 && save)
			return (free(buff), save);
		buff[read_bytes] = 0;
		save = ft_strjoin_gnl(save, buff);
		if (!save)
			return (free(buff), NULL);
	}
	free(buff);
	return (save);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*save[1096];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	save[fd] = ft_read_and_save(fd, save[fd]);
	if (!save[fd])
		return (NULL);
	line = ft_get_line(save[fd]);
	if (line == NULL)
		return (ft_free_static(save[fd]), save[fd] = NULL);
	save[fd] = ft_save(save[fd]);
	if (!save[fd])
	{
		free(line);
		return (NULL);
	}
	if (save[fd][0] == 0)
	{
		free(save[fd]);
		save[fd] = 0;
	}
	return (line);
}
