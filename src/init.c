#include "../inc/philo.h"

/*
	- gets current time using gettimeofday()
	- tv.tv_sec = seconds since jan 1, 1970
	- tv.tv_usec = microseconds
	- converts both to milliseconds and adds them
	- returns total milliseconds since epoch
*/
/*	Used for:
	- recording simulation start time
	- tracking when philosophers last ate
	- calculating elapsed time for logs
	- checking if time_to_die exceeded
*/
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
	1. allocates array of n_philo mutexes (one per fork)
	2. initializes each mutex with pthread_mutex_init()
	3. if any initialization fails:
		- destroys all previously initialized mutexes
		- free memory
		- returns error
*/
/* Why mutexes?
	- each fork is a shared resource
	- only one philosopher can hold a fork at a time
	- mutex prevents two philosophers from grabbing the same fork
*/
static int	init_forks(t_shared *shared, int n_philo)
{
	int	i;

	shared->forks = malloc(sizeof(pthread_mutex_t) * n_philo);
	if (!shared->forks)
		return (1);
	i = 0;
	while (i < n_philo)
	{
		if (pthread_mutex_init(&shared->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&shared->forks[i]);
			free(shared->forks);
			return (1);
		}
		i++;
	}
	return (0);
}

/*
	1. copies parameters to shared structure
	2. sets stop = 0 (simulation running)
	3. records start time with get_time_ms()
	4. calls init_forks() to create fork mutexes
	5. initializes print_mutex (protects printf from overlapping)
	6. initializes state_mutex (protects shared state changes)
*/
int	init_shared(t_shared *shared, t_params *params)
{
	shared->params = *params;
	shared->n_philo = params->n_philo;
	shared->stop = 0; // simulation running
	shared->start_time_ms = get_time_ms(); // record start time
	if (init_forks(shared, params->n_philo) != 0) // create fork mutexes
		return (1);
	if (pthread_mutex_init(&shared->print_mutex, NULL) != 0) // protects printf from overlapping
	{
		cleanup_forks(shared);
		return (1);
	}
	if (pthread_mutex_init(&shared->state_mutex, NULL) != 0) // protects shared state change
	{
		pthread_mutex_destroy(&shared->print_mutex);
		cleanup_forks(shared);
		return (1);
	}
	return (0);
}

/*
	1. allocated array of n_philo philosopher structures
	2. for each philospher (i = 0 to n_philo-1):
		- id = i + 1 (philosophers numbered 1 to n)
		- left_fork = i (philosopher i's left fork)
		- right_fork = (i+1) % n_philo (wraps around for circular table)
		- last_meal_ms = start_time_ms (all start having "just eaten")
		- meals_eaten = 0
		- shared pointer to shared resources
*/
t_philo	*init_philos(t_shared *shared)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * shared->n_philo);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < shared->n_philo)
	{
		philos[i].id = i + 1; // philosophers numbered 1 to n
		philos[i].left_fork = i; // philospher i's left fork
		philos[i].right_fork = (i + 1) % shared->n_philo; // wraps around for circular table
		philos[i].last_meal_ms = shared->start_time_ms; // all start having "just eaten"
		philos[i].meals_eaten = 0;
		philos[i].shared = shared; // pointer to shared resources
		i++;
	}
	return (philos);
}

/* fork arrangement for 5 philosophers
	philo 1: left_fork = 0, right_fork = 1
	philo 2: left_fork = 1, right_fork = 2
	philo 3: left_fork = 2, right_fork = 3
	philo 4: left_fork = 3, right_fork = 4
	philo 5: left_fork = 4, right_fork = 0 <- wraps around
*/