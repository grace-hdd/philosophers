#include "../inc/philo.h"

int	main(int ac, char **av)
{
	t_params	params;
	t_shared	shared;
	t_philo		*philos;

	//validate and store parameters
	if (parse_args(ac, av, &params) != 0)
		return (1);
	// create mutexes and shared data
	if (init_shared(&shared, &params) != 0)
		return (1);
	// create philosopher structures
	philos = init_philos(&shared);
	if (!philos)
	{
		cleanup_shared(&shared);
		return (1);
	}
	// chreat threads
	if (start_simulation(philos, &shared) != 0)
	{
		cleanup_philos(philos, &shared);
		cleanup_shared(&shared);
		return (1);
	}
	cleanup_philos(philos, &shared); // free philosopher memory
	cleanup_shared(&shared); // destroy mutexes, free memory
	return (0);
}
