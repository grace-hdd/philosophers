/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:49:49 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/06 13:49:49 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	validate_params(t_params *params, long n_philo)
{
	if (n_philo <= 0 || n_philo > 2147483647
		|| params->time_to_die <= 0 || params->time_to_die > 2147483647
		|| params->time_to_eat <= 0 || params->time_to_eat > 2147483647
		|| params->time_to_sleep <= 0 || params->time_to_sleep > 2147483647)
	{
		printf("Error: Invalid parameter value.\n");
		return (1);
	}
	params->n_philo = (int)n_philo;
	return (0);
}

static int	parse_optional_arg(int ac, char **av, t_params *params)
{
	long	meals_temp;

	if (ac == 6)
	{
		meals_temp = ft_atol(av[5]);
		if (meals_temp <= 0 || meals_temp > 2147483647)
		{
			printf("Error: Invalid number of meals.\n");
			return (1);
		}
		params->meals_required = (int)meals_temp;
		params->has_meals_required = 1;
	}
	else
		params->has_meals_required = 0;
	return (0);
}

int	parse_args(int ac, char **av, t_params *params)
{
	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	params->time_to_die = ft_atol(av[2]);
	params->time_to_eat = ft_atol(av[3]);
	params->time_to_sleep = ft_atol(av[4]);
	if (validate_params(params, ft_atol(av[1])) != 0)
		return (1);
	return (parse_optional_arg(ac, av, params));
}
