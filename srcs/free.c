/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/03 11:41:42 by aheister      #+#    #+#                 */
/*   Updated: 2021/09/20 20:18:29 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** ARRAY_AMOUNT - counts the amount of objects in an array
** FREE_ARRAY - frees an array of objects
** FREE_LISTS - frees a list
*/

#include "pipex.h"

int	array_amount(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

void	free_array(char **object)
{
	int	amount;

	amount = array_amount(object);
	while (amount > 0)
	{
		if (object[amount])
			free(object[amount]);
		amount--;
	}
	free(object);
}

void	free_list(t_input *input)
{
	ft_lstclear(&input->list_paths, free);
	ft_lstclear(&input->list_cmds, free);
}
