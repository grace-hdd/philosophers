#include "../inc/philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// TODO: implement philosopher logic
	if (philo->shared->n_philo == 1)
	{
		print_status(philo, "has taken a fork");
		usleep(philo->shared->params.time_to_die * 1000);
		return (NULL);
	}
	while (1)
	{
		
	}
	(void)philo;
	return (NULL);
}

int	start_simulation(t_philo *philos, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->n_philo)
	{
		if (pthread_create(&philos[i].thread, NULL,
				philosopher_routine, &philos[i]) != 0)
		{
			shared->stop = 1;
			while (--i >= 0)
				pthread_join(philos[i].thread, NULL);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < shared->n_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return (0);
}