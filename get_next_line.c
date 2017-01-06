/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alushenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/06 20:03:25 by alushenk          #+#    #+#             */
/*   Updated: 2017/01/06 20:03:28 by alushenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

t_fd	*get_fd(t_fd *list, int fd)
{
	while (list)
	{
		if (list->fd == fd)
			return (list);
		if (list->next)
			list = list->next;
		else if ((list->next = (t_fd *)malloc(sizeof(t_fd))))
		{
			list = list->next;
			list->fd = fd;
			list->next = NULL;
			return (list);
		}
	}
	if ((list = (t_fd *)malloc(sizeof(t_fd))))
	{
		list->fd = fd;
		list->next = NULL;
		return (list);
	}
	return (NULL);
}

void	move_buf(char *elem_buf, int len)
{
	char *temp;

	temp = ft_strdup(elem_buf + len + 1);
	ft_bzero(elem_buf, BUFF_SIZE + 1);
	ft_strcpy(elem_buf, temp);
	free(temp);
}

int		read_str(char *elem_buf, char **line, ssize_t bytes_read)
{
	int		len;
	char	*temp;

	len = 0;
	while (elem_buf[len] != '\n' && elem_buf[len])
		len++;
	if (!*line)
	{
		*line = ft_strnew(len);
		*line = ft_strncpy(*line, elem_buf, len);
	}
	else
	{
		temp = ft_strdup(*line);
		free(*line);
		*line = ft_strnew(ft_strlen(temp) + len);
		ft_strcpy(*line, temp);
		*line = ft_strncat(*line, elem_buf, len);
		free(temp);
	}
	if (len < bytes_read)
	{
		move_buf(elem_buf, len);
		return (1);
	}
	if (len == bytes_read)
		ft_bzero(elem_buf, BUFF_SIZE + 1);
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	static t_fd *list;
	t_fd		*elem;
	ssize_t		len;
	ssize_t		bytes_read;

	bytes_read = 0;
	*line = NULL;
	if (fd < 0 || line == NULL || BUFF_SIZE <= 0)
		return (-1);
	if (list)
		elem = get_fd(list, fd);
	else
	{
		list = get_fd(list, fd);
		elem = list;
	}
	if (*elem->buf)
	{
		len = 0;
		while (elem->buf[len] != '\n' && elem->buf[len])
			len++;
		*line = ft_strnew(len);
		*line = ft_strncpy(*line, elem->buf, len);
		move_buf(elem->buf, len);
	}
	while ((bytes_read = read(fd, elem->buf, BUFF_SIZE)))
	{
		if (read_str(elem->buf, line, bytes_read))
			return (1);
	}
	if (!*line)
		return (0);
	return (1);
}
