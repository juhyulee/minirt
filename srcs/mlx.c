/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siwolee <siwolee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:22:02 by juhyulee          #+#    #+#             */
/*   Updated: 2023/06/01 17:13:08 by siwolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

//editing
void	ft_init_mlx(t_vars *vars, t_scene *scene, t_data *image)
{
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, scene->canvas.width, \
	scene->canvas.height, "miniRT");
	image->img = mlx_new_image(vars->mlx, scene->canvas.width, \
	scene->canvas.height);
	image->addr = mlx_get_data_addr(image->img, &image->bits_per_pixel, \
	&image->line_length, &image->endian);
}

void	exit_error(int code)
{
	if (code == ERROR_NO_INPUT)
	{
		printf("no file input\n");
	}
	else if (code == ERROR_CANNOT_OPEN_FILE)
		printf("cannot open file\n");
	else if (code == ERROR_PARSE)
		printf("parsing error\n");
	else
		printf("error not specified yet : %d\n", code);
	exit(1);
}

int	file_check(int ac, char **av)
{
	int	fd;

	if (ac != 2)
		exit_error(ERROR_NO_INPUT);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
	{
		exit_error(2);
	}
	return (fd);
}

void *free_cam(t_camera *node)
{
	t_camera	*next;

	if(!node)
		return (NULL);
	while(node)
	{
		next = node->next;
		free(node);
		node = next;
	}
	return (NULL);
}

void *free_object(t_object *node)
{
	t_object	*next;

	if(!node)
		return (NULL);
	while(node)
	{
		next = node->next;
		free(node->element);
		free(node);
		node = next;
	}
	return (NULL);
}


// camera, world, light
void	free_vars(t_vars *vars)
{
	vars->scene.camera = free_cam(vars->scene.camera);
	vars->scene.world = free_object(vars->scene.world);
	vars->scene.light = free_object(vars->scene.light);
}

int	key_press(int keycode, t_vars *vars)
{
	t_vec			vup;

	if (keycode == 53 || keycode == X_EVENT_KEY_EXIT)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		free_vars(vars);
		exit(0);
	}
	vup = vars->scene.camera->orig;
	if (keycode == 13)
		vup = vec(vup.x, vup.y + VAL, vup.z);
	else if (keycode == 1)
		vup = vec(vup.x, vup.y - VAL, vup.z);
	else if (keycode == 0)
		vup = vec(vup.x + VAL, vup.y, vup.z);
	else if (keycode == 2)
		vup = vec(vup.x - VAL, vup.y, vup.z);
	else
		return (0);
	vprint("cam orig is", vup);
	move_camera(vars->scene.camera, vup);
	ft_draw(&vars->scene, &vars->image);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->image.img, 0, 0);
	return (0);
}
