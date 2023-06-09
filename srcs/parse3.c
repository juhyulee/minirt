/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siwolee <siwolee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:16:58 by juhyulee          #+#    #+#             */
/*   Updated: 2023/06/01 17:26:21 by siwolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	input_light(t_scene *scene, char *str)
{
	char		**split;
	char		**origin;
	char		**color;

	split = ft_split(str, ' ');
	if (!split[0] || !split[1] || !split[2] || !split[3] || split[4])
		exit_error(ERROR_PARSE);
	origin = ft_split(split[1], ',');
	color = ft_split(split[3], ',');
	oadd(&scene->light, object(LIGHT_POINT, light_point(parse_vec(origin), \
	vec(1, 1, 1), ft_atod(split[2])), parse_vec_normalize_color(color)));
	free_split(origin);
	free_split(color);
	free_split(split);
}

void	input_canvas(t_scene *scene, char *str)
{
	int		width;
	int 	height;
	char	**split;

	split = ft_split(str, ' ');
	if (!split[0] || !split[1] || !split[2] || split[3])
		exit_error(ERROR_PARSE);
	width = ft_atoi(split[1]);
	height = ft_atoi(split[2]);
	scene->canvas = canvas(width, height);
	free_split(split);
}

void	id_check(t_scene *scene, char *str)
{
	if (str[0] == 'A')
		input_ambient(scene, str);
	else if (str[0] == 'C')
		input_camera(scene, str);
	else if (str[0] == 'L')
		input_light(scene, str);
	else if (ft_strncmp(str, "pl", 2) == 0)
		input_plane(scene, str);
	else if (ft_strncmp(str, "sp", 2) == 0)
		input_sphere(scene, str);
	else if (ft_strncmp(str, "cy", 2) == 0)
		input_cylinder(scene, str);
	else if (ft_strncmp(str, "map", 3) == 0)
		input_canvas(scene, str);
}

void	readmap(t_scene *scene, int fd)
{
	char	*str;

	str = get_next_line(fd);
	if (!str)
		exit_error(ERROR_NO_INPUT);
	convert_space(str);
	while (str != 0)
	{
		id_check(scene, str);
		free(str);
		str = NULL;
		str = get_next_line(fd);
		convert_space(str);
	}
	if (!scene->camera)
		exit_error(10);
	free(str);
}