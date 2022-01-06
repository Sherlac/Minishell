/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmariot <cmariot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 15:00:15 by cmariot           #+#    #+#             */
/*   Updated: 2022/01/06 01:06:23 by cmariot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * token =	A sequence of characters considered a single unit by shell.
 *			It is either a word or an operator.
 *
 * word =	A sequence of characters treated as a unit by the shell.
 *			Words may not include unquoted metacharacters.
 *
 * metacharacter = A character that, when unquoted, separates words.
 *			A metacharacter is a space, tab, newline, or one of the following
 *			characters:
 *			‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
 *
 * operator = A control operator or a redirection operator.
 *			See Redirections, for a list of redirection operators.
 *			Operators contain at least one unquoted metacharacter.
 *
 * control_operator = A token that performs a control function.
 *			It is a newline or one of the following:
 *			‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.
 */

bool	is_blank(char *line, size_t i);
bool	is_a_metacharacter(char *line, size_t *i, bool opt);

/* count the len of the word, like in parse_word() */

size_t	len_of_word(char *line, size_t *i)
{
	size_t	len;
	size_t	i_backup;
	char	quote_type;
	size_t	i_plus_un;

	i_backup = *i;
	while (line[*i] != '\0' && line[*i] != '#')
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			quote_type = line[*i];
			(*i)++;
			while (line[*i] != '\0' && line[*i] != '#')
			{
				if (line[*i] == quote_type && is_blank(line, (*i) + 1) == TRUE)
					break ;
				i_plus_un = *i + 1;
				if (line[*i] == quote_type
					&& is_a_metacharacter(line, &i_plus_un, FALSE) == TRUE)
				{
					*i = i_plus_un;
					break ;
				}
				(*i)++;
			}
			if (line[*i] == quote_type)
				(*i)++;
			break ;
		}
		if (is_a_metacharacter(line, i, FALSE) == TRUE)
			break ;
		if (is_blank(line, *i) == TRUE)
			break ;
		(*i)++;
	}
	len = (*i) - i_backup;
	return (len);
}

/* return the len of the metacharacter which begings at line[i] */

size_t	len_of_metacharacter(char *line, size_t *i)
{
	if (line[*i] == '<' && line[*i + 1] && line[*i + 1] == '<')
	{
		(*i) += 2;
		return (2);
	}
	else if (line[*i] == '>' && line[*i + 1] && line[*i + 1] == '>')
	{
		(*i) += 2;
		return (2);
	}
	else if (line[*i] == '<' || line[*i] == '>')
		(*i) += 1;
	else if (line[*i] == '|' || line[*i] == '&' || line[*i] == ';')
		(*i) += 1;
	else if (line[*i] == '(' || line[*i] == ')' || line[*i] == '\n')
		(*i) += 1;
	return (1);
}

/* case 1 : metacharacter ; case 2 : word */

size_t	count_token_len(char *line, size_t *index)
{
	if (is_a_metacharacter(line, index, FALSE) == TRUE)
		return (len_of_metacharacter(line, index));
	else
		return (len_of_word(line, index));
}

/* skip the blank character, get the len of the tokens and substr */

char	*get_token_value(char *line, size_t *index)
{
	char	*token;
	size_t	token_len;
	size_t	first_index;

	if (is_blank(line, *index) == TRUE)
		while (is_blank(line, *index) == TRUE)
			(*index)++;
	first_index = *index;
	//printf("[%s]\n", line + *index);
	token_len = count_token_len(line, index);
	//printf("TOKEN_LEN = %lu\n", token_len);
	token = ft_substr(line, first_index, token_len);
	return (token);
}

/* a loop used to fill the array of tokens */

void	fill_tokens_array(char **array, char *line, size_t number_of_tokens)
{
	size_t	index;
	size_t	i;

	index = 0;
	i = 0;
	while (i < number_of_tokens)
	{
		array[i] = get_token_value(line, &index);
		i++;
	}
}

/* while the word isn't finished, i++ 
 * a word is finished when word[i] == blank or word[i] == metacharacter
 * if there is an opening quote in the word, the closing quote 
 * must be followed by ('\0', blank, metacharacter) */

void	parse_word(char *line, size_t *i)
{
	char	quote_type;
	size_t	i_plus_un;

	while (line[*i] != '\0' && line[*i] != '#')
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			quote_type = line[*i];
			(*i)++;
			while (line[*i] != '\0' && line[*i] != '#')
			{
				if (line[*i] == quote_type && is_blank(line, (*i) + 1) == TRUE)
					break ;
				i_plus_un = *i + 1;
				if (line[*i] == quote_type
					&& is_a_metacharacter(line, &i_plus_un, FALSE) == TRUE)
				{
					*i = i_plus_un;
					break ;
				}
				(*i)++;
			}
			if (line[*i] == quote_type)
				(*i)++;
			break ;
		}
		if (is_a_metacharacter(line, i, FALSE) == TRUE)
			break ;
		if (is_blank(line, *i) == TRUE)
			break ;
		(*i)++;
	}
}

/* return true if line[i] is a metacharacter,
 * if the option opt is TRUE the value of i is increased by
 * the len of the metacharacter */

bool	is_a_metacharacter(char *line, size_t *i, bool opt)
{
	bool	meta;

	if (line[*i] == '<' && line[*i + 1] && line[*i + 1] == '<')
	{
		if (opt == TRUE)
			(*i)++;
		meta = TRUE;
	}
	else if (line[*i] == '>' && line[*i + 1] && line[*i + 1] == '>')
	{
		if (opt == TRUE)
			(*i)++;
		meta = TRUE;
	}
	else if (line[*i] == '<' || line[*i] == '>')
		meta = TRUE;
	else if (line[*i] == '|' || line[*i] == '&' || line[*i] == ';')
		meta = TRUE;
	else if (line[*i] == '(' || line[*i] == ')' || line[*i] == '\n')
		meta = TRUE;
	else
		return (FALSE);
	if (opt == TRUE)
		(*i)++;
	return (meta);
}

/* return true if line[i] == space or tab */

bool	is_blank(char *line, size_t i)
{
	if (line[i] == ' ')
		return (TRUE);
	else if (line[i] == '\t')
		return (TRUE);
	else
		return (FALSE);
}

/* Count the number of tokens (= words + metacharacters), skip blank characters */

size_t	count_number_of_tokens(char *line)
{
	size_t	number_of_tokens;
	size_t	i;

	number_of_tokens = 0;
	i = 0;
	while (line[i] != '\0' && line[i] != '#')
	{
		if (is_blank(line, i) == TRUE)
			while (is_blank(line, i) == TRUE)
				i++;
		else
		{
			if (is_a_metacharacter(line, &i, TRUE) == TRUE)
				number_of_tokens++;
			else
			{
				parse_word(line, &i);
				number_of_tokens++;
			}
		}
	}
	return (number_of_tokens);
}

/* Count the number of tokens, allocate memory and fill the array */

char	**get_tokens_array(char *line)
{
	char	**array;
	size_t	number_of_tokens;

	if (line == NULL)
		return (NULL);
	number_of_tokens = count_number_of_tokens(line);
	//printf("NUMBER_OF_TOKENS = %lu\n", number_of_tokens);
	array = ft_calloc(number_of_tokens + 1, sizeof(char *));
	if (!array)
		return (NULL);
	fill_tokens_array(array, line, number_of_tokens);
	return (array);
}

/* Create an array of tokens */

char	**split_command_line(char *line)
{
	char	**first_array;
<<<<<<< HEAD

	first_array = get_tokens_array(line);
	return (first_array);
=======
	char	**second_array;
	char	**third_array;
	char	**fourth_array;
	char	**final_array;

	first_array = split_line(line, " \t|><\"\';");
	ft_putarray("first", first_array);
	if (!first_array)
		return (NULL);
	second_array = join_heredoc(first_array);
	ft_putarray("second", second_array);
	ft_free_array(first_array);
	if (!second_array)
		return (NULL);
	third_array = join_quote(second_array);
	ft_putarray("third", third_array);
	ft_free_array(second_array);
	if (!second_array)
		return (NULL);
	fourth_array = trim_quote_space_del(third_array);
	ft_free_array(third_array);
	if (!third_array)
		return (NULL);
	final_array = remove_comments(fourth_array);
	ft_putarray("final", final_array);
	return (final_array);
>>>>>>> main
}
