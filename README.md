# Philosophers

This project is an implementation of the Dining Philosophers problem, a classic synchronization problem in computer science. It is designed to illustrate the challenges of avoiding deadlock and starvation in a concurrent system.

The project is divided into two parts:
- **Mandatory Part (`philo/`):** Solves the problem using threads and mutexes.
- **Bonus Part (`philo_bonus/`):** Solves the problem using processes and semaphores.

## Project Overview

A number of philosophers are sitting at a round table. In the center of the table is a large bowl of spaghetti. Between each pair of philosophers is a single fork.

- To eat, a philosopher must pick up both the fork to their left and the fork to their right.
- A philosopher can only eat, think, or sleep.
- The simulation ends if a philosopher dies of starvation or, optionally, if all philosophers have eaten a specified number of times.

## Compilation

A root `Makefile` is provided to manage both the mandatory and bonus parts.

### Mandatory Part

To compile the mandatory version (`philo`), run:
```shell
make
```

### Bonus Part

To compile the bonus version (`philo_bonus`), run:
```shell
make bonus
```

### Cleaning

- `make clean`: Removes the object files for both versions.
- `make fclean`: Removes the object files and the executables for both versions.
- `make re`: Cleans and recompiles the mandatory part.
- `make re_bonus`: Cleans and recompiles the bonus part.

## Usage

Both programs take the same set of command-line arguments.

```shell
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

### Arguments

- `number_of_philosophers`: The number of philosophers (and forks).
- `time_to_die` (ms): If a philosopher doesn't start eating within this time since their last meal, they die.
- `time_to_eat` (ms): The time it takes for a philosopher to eat.
- `time_to_sleep` (ms): The time a philosopher will spend sleeping.
- `number_of_times_each_philosopher_must_eat` (optional): The simulation stops after every philosopher has eaten this many times.

### Example

```shell
./philo_bonus 5 800 200 200
```