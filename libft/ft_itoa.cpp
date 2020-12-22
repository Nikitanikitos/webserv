#include "libft.hpp"

static int	ft_len(int n)
{
	unsigned char		i;

	i = 0;
	if (n == 0)
		return (1);
	else if (n < 0)
		i++;
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char*		ft_itoa(int n) {
	unsigned char		i;
	char				*result;
	char				pos_or_neg;

	i = ft_len(n);
	pos_or_neg = 1;
	result = new char[i + 1];
	result[i--] = '\0';
	if (n == 0)
		result[i--] = '0';
	else if (n < 0) {
		result[0] = '-';
		pos_or_neg = -1;
	}
	while (n) {
		result[i--] = ((n % 10) * pos_or_neg) + 48;
		n /= 10;
	}
	return (result);
}