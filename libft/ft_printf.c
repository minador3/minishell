/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:11:51 by mwei              #+#    #+#             */
/*   Updated: 2026/01/17 16:45:23 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_conversion(char lettr, va_list args)
{
	int	result;

	if (lettr == 'c')
		result = ft_putchar(va_arg(args, int));
	else if (lettr == 's')
		result = ft_putstr(va_arg(args, char *));
	else if (lettr == 'p')
		result = ft_pointer(va_arg(args, void *));
	else if (lettr == 'd' || lettr == 'i')
		result = ft_putnbr(va_arg(args, int));
	else if (lettr == 'u')
		result = ft_put_unsigned(va_arg(args, unsigned int));
	else if (lettr == 'x')
		result = ft_put_hex(va_arg(args, unsigned int), 0);
	else if (lettr == 'X')
		result = ft_put_hex(va_arg(args, unsigned int), 1);
	else if (lettr == '%')
		result = ft_putchar('%');
	else
		return (-1);
	return (result);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		written;
	int		result;

	va_start(args, str);
	written = 0;
	while (*str)
	{
		result = 0;
		if (*str == '%' && *(str + 1) != '\0')
		{
			str++;
			result = ft_conversion(*str, args);
		}
		else if (*str != '%')
			result = write(1, str, 1);
		if (result == -1)
			return (va_end(args), -1);
		written += result;
		str++;
	}
	return (va_end(args), written);
}
