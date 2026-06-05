/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:00:50 by mwei              #+#    #+#             */
/*   Updated: 2026/01/17 16:45:34 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_put_hex(unsigned long long n, int uppercase)
{
	int		written;
	int		result;
	char	*base;
	char	digit;

	if (uppercase == 1)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	written = 0;
	if (n >= 16)
	{
		result = ft_put_hex(n / 16, uppercase);
		if (result < 0)
			return (result);
		written += result;
	}
	digit = base[n % 16];
	result = write(1, &digit, 1);
	if (result == -1)
		return (-1);
	written += result;
	return (written);
}
