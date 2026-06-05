/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:59:26 by mwei              #+#    #+#             */
/*   Updated: 2026/01/17 16:45:48 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int c)
{
	long	num;
	int		written;
	int		result;

	num = c;
	written = 0;
	if (num < 0)
	{
		result = ft_putchar('-');
		if (result == -1)
			return (-1);
		written += result;
		num = -num;
	}
	if (num >= 10)
	{
		result = ft_putnbr(num / 10);
		if (result < 0)
			return (result);
		written += result;
	}
	if (ft_putchar((num % 10) + '0') == -1)
		return (-1);
	written++;
	return (written);
}
