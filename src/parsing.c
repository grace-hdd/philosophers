#include "../inc/philo.h"

// checks if character is digit
static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

// checks if charaacter is a white space
static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

// converts string to long
static long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

// takes command-line args and fills t_params
/*
	- correct number of arguments (5 or 6)
	- all values are positive
	- if meals_required is provided, it must be positive
*/
int	parse_args(int ac, char **av, t_params *params)
{
	if (ac != 5 && ac != 6)
	{
		printf("Error\n");
		return (1);
	}
	params->n_philo = (int)ft_atol(av[1]);
	params->time_to_die = ft_atol(av[2]);
	params->time_to_eat = ft_atol(av[3]);
	params->time_to_sleep = ft_atol(av[4]);
	if (ac == 6)
	{
		params->meals_required = (int)ft_atol(av[5]);
		params->has_meals_required = 1;
	}
	else
		params->has_meals_required = 0;
	if (params->n_philo <= 0 || params->time_to_die <= 0
		|| params->time_to_eat <= 0 || params->time_to_sleep <= 0)
		return (1);
	if (params->has_meals_required && params->meals_required <= 0)
		return (1);
	return (0);
}
