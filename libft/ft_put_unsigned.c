/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_unsigned.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:01:10 by mwei              #+#    #+#             */
/*   Updated: 2026/01/17 16:45:38 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_put_unsigned(unsigned int n)
{
	int		written;
	int		result;
	char	digit;

	result = 0;
	written = 0;
	if (n >= 10)
	{
		result = ft_put_unsigned(n / 10);
		if (result < 0)
			return (result);
		written += result;
	}
	digit = (n % 10) + '0';
	result = write(1, &digit, 1);
	if (result == -1)
		return (-1);
	written += result;
	return (written);
}
