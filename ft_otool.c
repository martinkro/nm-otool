/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glasset <glasset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/05 15:02:36 by glasset           #+#    #+#             */
/*   Updated: 2016/10/31 16:11:49 by glasset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		map(struct stat buf, int fd, char *filename)
{
	char		*ptr;

	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
	{
		write(2, "fail mmap\n", 10);
		return (0);
	}
	otool(ptr, filename);
	if (munmap(0, buf.st_size) < 0)
	{
		write(2, "fail munmap\n", 12);
		return (0);
	}
	return (1);
}

static int		map_file(char *file_name)
{
	int			fd;
	struct stat	buf;

	if ((fd = open(file_name, O_RDONLY)) < 0)
	{
		write(2, "fail to open\n", 13);
		return (0);
	}
	if (fstat(fd, &buf) < 0)
	{
		write(2, "fail fstat\n", 11);
		return (0);
	}
	if (!map(buf, fd, file_name))
		return (0);
	close(fd);
	return (1);
}

int				loop_file(int ac, char **av)
{
	int			i;

	i = 1;
	while (i < ac)
	{
		if (av[i][strlen(av[i]) - 1] == 'a')
		{
			ft_putstr("Archive : ");
			ft_putstr(av[i]);
			ft_putchar('\n');
		}
		else
		{
			ft_putstr(av[i]);
			write(1, ":\n", 2);
		}
		if (!map_file(av[i++]))
			write(1, "KO", 2);
	}
	return (1);
}

int				main(int ac, char **av)
{
	if (ac < 2)
		write(2, "need arg\n", 12);
	else
		loop_file(ac, av);
	return (0);
}
