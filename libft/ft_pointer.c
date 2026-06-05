/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:07:19 by mwei              #+#    #+#             */
/*   Updated: 2026/01/17 16:45:19 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_pointer(void *ptr)
{
	unsigned long long	address;
	int					written;
	int					result;

	written = 2;
	result = write(1, "0x", 2);
	if (result == -1)
		return (-1);
	address = (unsigned long long)ptr;
	result = ft_put_hex(address, 0);
	if (result == -1)
		return (-1);
	written += result;
	return (written);
}
