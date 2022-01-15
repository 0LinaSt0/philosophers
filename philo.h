/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalena <msalena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 18:03:52 by msalena           #+#    #+#             */
/*   Updated: 2022/01/15 14:57:22 by msalena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define DIE 1
# define PAUSE 120
# define PAUSE_MAIN 120
# define PAUSE_MUTEX 120

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include "libft/libft.h"


typedef struct	s_argv
{
	int	phil_num; /*and mutex_num too*/
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	how_much_eats; /*number_of_times_each_philosopher_must_eat: if -1 => without this argument*/
}		t_argv;

typedef struct	s_philo
{
	int				num;
	pthread_t		thread;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	pthread_mutex_t	*printing;
	t_argv			*argums;
	struct timeval	*t_start; // maybe segfault
	struct timeval	t_eat;
	int				die_fl;
}					t_philo;


/*doing_statuses*/

int		do_take_fork(t_philo *phil, char fl);
int		do_eat(t_philo *phil);
int		do_sleeping(t_philo *phil);
int		do_thinking(t_philo *phil);
int		do_check_die(t_philo *phil);

/*utils*/

long	actual_time(struct timeval *start_time);
void	ft_usleep(int mlsec);
long	tmp_micsec_AT(struct timeval *start_time);//tmp/del after

#endif
