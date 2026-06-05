/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:02:32 by mwei              #+#    #+#             */
/*   Updated: 2026/01/17 16:47:53 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(const char *str)
{
	int	len;
	int	written;

	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	written = write(1, str, len);
	if (written == -1)
		return (-1);
	return (len);
}
