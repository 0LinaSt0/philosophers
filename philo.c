/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalena <msalena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 17:21:54 by msalena           #+#    #+#             */
/*   Updated: 2022/01/15 14:57:58 by msalena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*ARGUMENTS:
	number_of_philosophers
	time_to_die
	time_to_eat
	time_to_sleep
	[number_of_times_each_philosopher_must_eat]*/

int	check_error(int argc, char **argv)
{
	int	i_arr;
	int	i_str;

	i_arr = 1;
	if (argc != 5 && argc != 6)
	{
		printf ("Error arguments: try again with other ones\n");
		return (-1);
	}
	while (argv[i_arr])
	{
		i_str = 0;
		while (argv[i_arr][i_str])
		{
			if (!ft_isdigit(argv[i_arr][i_str]))
			{
				printf ("Error arguments: try again with other ones\n");
				return (-1);
			}
			i_str++;
		}
		i_arr++;
	}
	return (0);
}

int	char_to_num(int argc, char **argv, t_argv *argums)
{
	int	i_arr;
	int	i_str;

	i_arr = 0;
	i_str = 0;
	if (check_error(argc, argv) == -1)
		return (-1);
	argums->phil_num = ft_atoi(argv[1]);
	argums->die_time = ft_atoi(argv[2]);
	argums->eat_time = ft_atoi(argv[3]);
	argums->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		argums->how_much_eats = ft_atoi(argv[5]);
	else
		argums->how_much_eats = -1;
	return (0);
}

/*1 second == 1000 mlsecond
1 microse == 0,001 mlsecond*/
void	*start(void *struct_)
{
	t_philo			*phil;

	phil = (t_philo *)struct_;
	while (1 && phil->argums->how_much_eats)
	{
		if (do_thinking(phil) == DIE)
			return (NULL);

			///////////////////
			// pthread_mutex_lock(phil->printing);
			//        printf("/////////////////time:%ld phil_num:%d l_ptr:%p r_ptr:%p\n",
			// 	          tmp_micsec_AT(phil->t_start),phil->num, phil->left, phil->right);
			// pthread_mutex_unlock(phil->printing);
			///////////////////

		if ((phil->num % 2) != 0)
		{
			// ft_usleep(PAUSE_MUTEX);
			pthread_mutex_lock(phil->left);
			// ft_usleep(PAUSE_MUTEX);
			pthread_mutex_lock(phil->right);
			///////////////////
			// pthread_mutex_lock(phil->printing);
			//        printf("~~~~~~~~~~~~~~~~time:%ld phil_num:%d l_ptr:%p r_ptr:%p\n",
			// 	          tmp_micsec_AT(phil->t_start),phil->num, phil->left, phil->right);
			// pthread_mutex_unlock(phil->printing);
			///////////////////
			if (do_take_fork(phil, 'R') == DIE)
				return (NULL);
			if (do_take_fork(phil, 'L') == DIE)
				return (NULL);
			if (do_eat(phil) == DIE)
				return (NULL);
			ft_usleep(phil->argums->eat_time);
			pthread_mutex_unlock(phil->left);
			pthread_mutex_unlock(phil->right);
			///////////////////
			// pthread_mutex_lock(phil->printing);
			//        printf("_~~~~~~~~~~~time:%ld phil_num:%d l_ptr:%p r_ptr:%p\n",
			// 	          tmp_micsec_AT(phil->t_start),phil->num, phil->left, phil->right);
			// pthread_mutex_unlock(phil->printing);
			///////////////////
		}
		else
		{
			usleep(PAUSE_MUTEX);
			pthread_mutex_lock(phil->right);
			usleep(PAUSE_MUTEX);
			pthread_mutex_lock(phil->left);
			///////////////////
			// pthread_mutex_lock(phil->printing);
			//        printf("------------------time:%ld phil_num:%d l_ptr:%p r_ptr:%p\n",
			// 	          tmp_micsec_AT(phil->t_start),phil->num, phil->left, phil->right);
			// pthread_mutex_unlock(phil->printing);
			///////////////////
			if (do_take_fork(phil, 'L') == DIE)
				return (NULL);
			if (do_take_fork(phil, 'R') == DIE)
				return (NULL);
			if (do_eat(phil) == DIE)
				return (NULL);
			ft_usleep(phil->argums->eat_time);
			pthread_mutex_unlock(phil->right);
			pthread_mutex_unlock(phil->left);
			///////////////////
			// pthread_mutex_lock(phil->printing);
			//        printf("~~~~~~~~~~~time:%ld phil_num:%d l_ptr:%p r_ptr:%p\n",
			// 	          tmp_micsec_AT(phil->t_start),phil->num, phil->left, phil->right);
			// pthread_mutex_unlock(phil->printing);
			//////////////////
		}
		if (do_sleeping(phil) == DIE)
			return (NULL);
		if (phil->argums->how_much_eats)
			phil->argums->how_much_eats--;
	}
	return (NULL);
}

int	main_thread(t_philo *philo)
{
	int	i;
	int	j;
	int	phil_num;

	i = 0;
	j = 0;
	phil_num = (philo + i)->argums->phil_num;
	while (1)
	{
		while (i < phil_num)
		{
			if (actual_time(&((philo + i)->t_eat)) >= (philo + i)->argums->die_time)
			{
			// printf ("i:%d cur:%ld die:%d\n", i, actual_time(&((philo + i)->t_eat)), (philo + i)->argums->die_time);
				while (j < phil_num)
				{
					(philo + j)->die_fl = DIE;
					j++;
				}
				printf("time:%ld phil_num:%d died\n", actual_time(philo->t_start),(philo + i)->num);
				return (DIE);
			}
			i++;
		}
		usleep (PAUSE_MAIN);
		i = 0;
	}
	return (0);
}

int	open_threads(t_philo *philos)
{
	int	i;
	// struct timeval new;

	i = 0;
	// gettimeofday(&new, NULL);
	while (i < philos->argums->phil_num)
	{
					    //    printf("---------------time:%ld phil_num:%d\n",
				        //   tmp_micsec_AT(&new),(philos + i)->num);
		// gettimeofday(&new, NULL);
		pthread_create(&(philos + i)->thread, NULL, start, (philos + i));
		pthread_detach((philos + i)->thread);
		i++;
	}
	if (main_thread(philos) == DIE)
	{
		//free all
		return (DIE);
	}
	return (0);
}

int	initialization(t_argv *argums)
{
	struct timeval	cur_time;
	pthread_mutex_t	*mute_arr;
	pthread_mutex_t	print_fl;
	t_philo			*philo_arr;
	int				i;

	i = 0;
	mute_arr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * argums->phil_num);
	philo_arr = (t_philo *)malloc(sizeof(t_philo) * argums->phil_num);
	if (!mute_arr || !philo_arr)
	{
		free(mute_arr);
		free(philo_arr);
		return (DIE);
	}
	gettimeofday(&cur_time, NULL);

	philo_arr[i].num = i + 1;
	philo_arr[i].right = &(mute_arr[argums->phil_num - 1]);
	philo_arr[i].left = &(mute_arr[i]);
	philo_arr[i].argums = argums;
	philo_arr[i].t_start = &cur_time;
	philo_arr[i].t_eat = cur_time;
	philo_arr[i].die_fl = 0;
	(philo_arr + i)->printing = &print_fl;
	pthread_mutex_init((philo_arr + i)->printing, NULL);
	i++;
	while (i < argums->phil_num)
	{
		pthread_mutex_init(&(mute_arr[i]), NULL);
		(philo_arr + i)->printing = &print_fl;
		pthread_mutex_init((philo_arr + i)->printing, NULL);
		philo_arr[i].num = i + 1;
		philo_arr[i].right = &(mute_arr[i - 1]);
		philo_arr[i].left = &(mute_arr[i]);
		philo_arr[i].argums = argums;
		philo_arr[i].t_start = &cur_time;
		philo_arr[i].t_eat = cur_time;
		philo_arr[i].die_fl = 0;
		i++;
	}
	if (open_threads(philo_arr) == DIE)
		// return (DIE);
	// return (0);
	for(int i=0; i<argums->phil_num; i++){
		printf("#:%d  -> %p\n", i, mute_arr + i);
	}

	for(int i=0; i<argums->phil_num; i++){
		printf("num:%d     ->     R - %p     L - %p\n", philo_arr[i].num, philo_arr[i].right, philo_arr[i].left);
	}
	return (0);
}

int	main (int argc, char **argv)
{
	t_argv arg;
	char_to_num(argc, argv, &arg);
	// printf("phil_num = %d\ndie_time = %d\neat_time = %d\nsleep_time=%d\n", arg.phil_num, arg.die_time, arg.eat_time, arg.sleep_time);

	/*
	struct timeval begin;
	gettimeofday(&begin, 0);
	usleep(500 * 1000);
	printf("%ld\n", actual_time(&begin));
	*/

	/*
	struct timeval begin;
	gettimeofday(&begin, 0);
	ft_usleep(5);
	printf("%ld\n", tmp_micsec_AT(&begin));
	*/

	// return 0;

	t_argv	argums;

	if (char_to_num(argc, argv, &argums) < 0 || initialization(&argums))
		return (DIE);

	// printf ("%d\n", argums.phil_num);
	// printf ("%d\n", argums.die_time);
	// printf ("%d\n", argums.eat_time);
	// printf ("%d\n", argums.sleep_time);
	// printf ("%d\n", argums.how_much_eats);

	// while (){
	// 	pthread_mutexattr_init(arr[i]);
	// }0

}