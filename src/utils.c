#include "../inc/philo.h"

void	print_status(t_philo *philo, char *status)
{
	long	elapsed;

	elapsed = get_time_ms() - philo->shared->start_time_ms;
	pthread_mutex_lock(&philo->shared->print_mutex);
	if (!philo->shared->stop)
		printf("%ld %d %s\n", elapsed, philo->id, status);
	pthread_mutex_unlock(&philo->shared->print_mutex);	
}
