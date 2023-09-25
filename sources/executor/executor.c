/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmorvan <evmorvan@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 06:43:32 by evmorvan          #+#    #+#             */
/*   Updated: 2023/09/25 09:09:35 by evmorvan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	executor(t_ast_node *node, t_env *env)
{
	if (node == NULL)
		return ;
	if (node->type == ND_CMD)
		execute_command(node, env);
	else if (node->type == ND_PIPE)
		execute_pipe(node, env);
}

int	execute_special_builtins(t_ast_node *node, t_env *env)
{
	int	ret;

	ret = 0;
	if (ft_strcmp(node->cmd_name, "unset") == 0)
	{
		ret = 1;
		sh_unset(node, env);
	}
	else if (ft_strcmp(node->cmd_name, "export") == 0)
	{
		ret = 1;
		sh_export(node, env);
	}
	else if (ft_strcmp(node->cmd_name, "exit") == 0)
	{
		ret = 1;
		sh_exit(node);
	}
	else if (ft_strcmp(node->cmd_name, "cd") == 0)
	{
		ret = 1;
		sh_cd(node, env);
	}
	return (ret);
}

void	execute_command(t_ast_node *node, t_env *env)
{
	int		status;
	pid_t	pid;
	char	*tmp;

	if (execute_special_builtins(node, env))
		return ;
	pid = fork();
	if (pid == 0)
	{
		setup_redirections(node);
		launch_process(node, env);
	}
	else
	{
		node->cmd_pid = pid;
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
			tmp = ft_itoa((128 + WTERMSIG(status)));
		else
			tmp = ft_itoa(WEXITSTATUS(status));
		if (ft_strcmp(tmp, "139") == 0)
			ft_printf("Segmentation fault\n");
		env_set(env, "?", tmp);
		free(tmp);
	}
}

void	execute_pipe(t_ast_node *node, t_env *env)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) < 0)
		return (error(node->cmd_name, "pipe failed"));
	pid1 = fork();
	if (pid1 < 0)
		return (error(node->cmd_name, "fork failed"));
	if (pid1 == 0)
		pidis0(node, env, pipefd, STDOUT_FILENO);
	else
	{
		pid2 = fork();
		if (pid2 < 0)
			return (error(node->cmd_name, "fork failed"));
		if (pid2 == 0)
			pidis0(node, env, pipefd, STDIN_FILENO);
		else
		{
			closepipes(pipefd);
			wait(NULL);
			wait(NULL);
		}
	}
}

void	launch_process(t_ast_node *node, t_env *env)
{
	char	**args;
	int		ret;
	char	*path;
	char	**formatted_env;

	args = build_argv(node);
	execute_builtins(node, env, args);
	if (!get_exec_path_from_env(args[0], env))
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n",
			args[0]);
		exit(127);
	}
	path = get_exec_path_from_env(args[0], env);
	formatted_env = env_to_envp_format(env);
	ret = execve(path, args, formatted_env);
	free_split(formatted_env);
	free(path);
	free_split(args);
	if (ret == -1)
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", args[0],
			strerror(errno));
	exit(1);
}
