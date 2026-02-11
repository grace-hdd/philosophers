/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:49:53 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/06 13:49:53 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	grab_forks(t_philo *philo)
{
	int	first_fork;
	int	second_fork;
	int	locked_first;

	/* Keep same ordering by parity */
	if (philo->id % 2 == 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	else
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	locked_first = 0;
	while (1)
	{
		/* Check for stop request */
		pthread_mutex_lock(&philo->shared->state_mutex);
		if (philo->shared->stop)
		{
			pthread_mutex_unlock(&philo->shared->state_mutex);
			if (locked_first)
				pthread_mutex_unlock(&philo->shared->forks[first_fork]);
			return ;
		}
		pthread_mutex_unlock(&philo->shared->state_mutex);

		if (!locked_first)
		{
			if (pthread_mutex_trylock(&philo->shared->forks[first_fork]) == 0)
			{
				locked_first = 1;
				print_status(philo, "has taken a fork");
			}
			else
				usleep(100);
		}
		else
		{
			/* Try to get second fork */
			if (pthread_mutex_trylock(&philo->shared->forks[second_fork]) == 0)
			{
				print_status(philo, "has taken a fork");
				return ;
			}
			else
			{
				/* Couldn't get second, release first and retry */
				pthread_mutex_unlock(&philo->shared->forks[first_fork]);
				locked_first = 0;
				usleep(100);
			}
		}
	}
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->state_mutex);
	philo->last_meal_ms = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->shared->state_mutex);
	print_status(philo, "is eating");
	responsive_sleep(philo->shared, philo->shared->params.time_to_eat);
}

static void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->shared->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->shared->forks[philo->right_fork]);
}

static void	*philo_loop(t_philo *philo)
{
	int	should_stop;

	while (1)
	{
		pthread_mutex_lock(&philo->shared->state_mutex);
		should_stop = philo->shared->stop;
		pthread_mutex_unlock(&philo->shared->state_mutex);
		if (should_stop)
			break ;
		print_status(philo, "is thinking");
		grab_forks(philo);
		eat(philo);
		release_forks(philo);
		print_status(philo, "is sleeping");
		responsive_sleep(philo->shared, philo->shared->params.time_to_sleep);
	}
	return (NULL);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->shared->n_philo == 1)
	{
		print_status(philo, "has taken a fork");
		responsive_sleep(philo->shared, philo->shared->params.time_to_die);
		return (NULL);
	}
	philo_loop(philo);
	return (NULL);
}
