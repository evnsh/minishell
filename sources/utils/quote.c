/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmorvan <evmorvan@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:41:11 by evmorvan          #+#    #+#             */
/*   Updated: 2023/10/02 00:29:32 by evmorvan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_quotes(t_token *token)
{
	char	*result;
	int		i;
	int		j;
	int		quote_type;

	i = 0;
	j = 0;
	quote_type = 0;
	result = malloc(sizeof(char) * ft_strlen(token->token) + 1);
	while (token->token[i])
	{
		if (token->token[i] == '\'' || token->token[i] == '\"')
		{
			quote_type = token->token[i++];
			while (token->token[i] && token->token[i] != quote_type)
				result[j++] = token->token[i++];
		}
		else
			result[j++] = token->token[i];
		i++;
	}
	result[j] = '\0';
	free(token->token);
	token->token = result;
	token->quote_type = quote_type;
}

void	dequotter2000(t_token *tokens)
{
	while (tokens)
	{
		remove_quotes(tokens);
		tokens = tokens->next;
	}
}

int	is_directory(char *filepath)
{
	int	fd;

	fd = open(filepath, O_DIRECTORY, 0644);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	is_executable(char *filepath)
{
	int	fd;

	fd = open(filepath, X_OK, 0644);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}
