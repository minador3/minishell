/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:18:02 by mwei              #+#    #+#             */
/*   Updated: 2026/02/27 14:20:43 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*free_all(char *buffer, char *unread_data)
{
	free (buffer);
	free (unread_data);
	return (NULL);
}

static char	*read_until_newline(int fd, char *unread_data)
{
	char	*buffer;
	int		bytes_read;
	char	*temp;

	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while ((!unread_data || !gnl_strchr(unread_data, '\n')) && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free_all(buffer, unread_data));
		buffer[bytes_read] = '\0';
		temp = gnl_strjoin(unread_data, buffer);
		if (!temp)
			return (free_all(buffer, unread_data));
		free(unread_data);
		unread_data = temp;
	}
	free (buffer);
	buffer = NULL;
	return (unread_data);
}

static char	*extract_line(char *unread_data)
{
	size_t	line_len;
	char	*line;

	line_len = 0;
	if (!unread_data || *unread_data == '\0')
		return (NULL);
	while (unread_data[line_len] && unread_data[line_len] != '\n')
		line_len++;
	if (unread_data[line_len] == '\n')
		line_len++;
	line = gnl_substr(unread_data, 0, line_len);
	return (line);
}

static char	*update_unread_data(char *unread_data)
{
	size_t	i;
	char	*new_data;

	if (!unread_data || *unread_data == '\0')
	{
		free(unread_data);
		return (NULL);
	}
	i = 0;
	while (unread_data[i] && unread_data[i] != '\n')
		i++;
	new_data = gnl_substr(unread_data, i + 1, gnl_strlen(unread_data) - i - 1);
	free(unread_data);
	if (!new_data || *new_data == '\0')
	{
		free(new_data);
		new_data = NULL;
	}
	return (new_data);
}

char	*get_next_line(int fd)
{
	static char	*unread_data;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (NULL);
	unread_data = read_until_newline(fd, unread_data);
	if (!unread_data || *unread_data == '\0')
	{
		free(unread_data);
		unread_data = NULL;
		return (NULL);
	}
	line = extract_line(unread_data);
	if (line == NULL)
	{
		free(unread_data);
		unread_data = NULL;
	}
	else
		unread_data = update_unread_data(unread_data);
	return (line);
}
