/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 09:52:28 by flee              #+#    #+#             */
/*   Updated: 2021/12/24 21:28:45 by cmariot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Minishell should not interpret unclosed quotes
int	check_quote(char *line)
{
	int		i;
	char	c;
	int		ok;

	i = 0;
	ok = 1;
	while (line[i] != '\0')
	{
		if ((line[i] == '\"' || line[i] == '\''))
		{
			ok = 0;
			c = line[i];
			i++;
			while (line[i] != c && line[i] != '\0')
				i++;
			if (line[i] == c)
				ok = 1;
		}
		if (line[i] != 0)
			i++;
	}
	if (ok)
		return (1);
	printf("Minishell does not interpret unclosed quotes.\n");
	return (0);
}
