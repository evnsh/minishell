/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmorvan <evmorvan@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 10:59:11 by evmorvan          #+#    #+#             */
/*   Updated: 2023/06/22 15:58:16 by evmorvan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

	(void)argc;
	(void)argv;
	env = ft_env(envp);
	print_env(env);
	while (TRUE)
	{
		line = readline(PROMPT);
		if (line[0] != '\0')
			add_history(line);
		free(line);
	}
	free(line);
	return (0);
}
