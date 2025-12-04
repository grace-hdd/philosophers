#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_params
{
    int		n_philo;				// Number of philosophers
    long	time_to_die;			// Max time without eating (ms)
    long	time_to_eat;			// Time to eat (ms)
    long	time_to_sleep;			// Time to sleep (ms)
    int		meals_required;			// optional : meals each philosopher must eat
    int		has_meals_required;		// 1 if above is set, 0 otherwise
}	t_params;

typedef struct s_shared
{
    pthread_mutex_t	*forks;				// array of fork mutexes
    pthread_mutex_t	print_mutex;		// protects printf from overlapping
    pthread_mutex_t	state_mutex;		// protects shared state changes
    long			start_time_ms;		// simulation start timestamp
    int				n_philo;			// number of philosophers
    int				stop;				// stop flag (1 = stop simulation)
    t_params		params;				// copy of parameters
}	t_shared;

typedef struct s_philo
{
    int			id;					// philosopher number (1 to n_philo)
    int			left_fork;			// index of left fork mutex
    int			right_fork;			// index of right fork mutex
    long		last_meal_ms;		// timestamp of last meal start
    int			meals_eaten;		// count of meals eaten
    pthread_t	thread;				// the thread running this philosopher
    t_shared	*shared;			// pointer to shared resources
}	t_philo;

/* PARSING */
int		parse_args(int ac, char **av, t_params *params);

/* INITIALIZATION */
int		init_shared(t_shared *shared, t_params *params);
t_philo	*init_philos(t_shared *shared);

/* SIMULATION */
int		start_simulation(t_philo *philos, t_shared *shared);
void	*philosopher_routine(void *arg);

/* UTILITIES */
long	get_time_ms(void);
void	print_status(t_philo *philo, char *status);

/* CLEANUP */
void	cleanup_philos(t_philo *philos, t_shared *shared);
void	cleanup_shared(t_shared *shared);
void	cleanup_forks(t_shared *shared);

#endif