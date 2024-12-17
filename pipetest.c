#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
/*
void	error(void)
{
	perror("\033[31mError");
	exit(EXIT_FAILURE);
}
char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	path = NULL;
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			break ;
		free(path);
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (path);
}
// example we pass  ("ls -a", env)
void exec_path(char *full_cmd, char **env)
{
	char **cmd;
	char *path;
	int i; 

    i = -1;
	cmd = ft_split(full_cmd, ' ');
    if (!cmd)
        return;
	path = find_path(cmd[0], env);
	if (!path)
	{
		while(cmd[i++])
			free(cmd[i]);
		free(cmd);
		return;
	}
	if (execve(path,cmd,env) == -1)
	{
		free(path);
		while(cmd[++i])
			free(cmd[i]);
		free(cmd);
		return;
	}
}*/
// build the thing
int main(int ac, char *av[], char *env[]) 
{
  int fd[2];
  char str[7];
  int bt_read;
  
  if (pipe(fd) == -1)
    return 2;
  write(fd[1],"hello\n",6);
  printf("fd[1] =>%d\nfd[0] =>%d\n",fd[1],fd[0]);
  bt_read = read(fd[0],str,6);
  if (bt_read > 0)
    printf("%s",str);
  printf("bt_read =>%d\n",bt_read);
  close(fd[1]);
  close(fd[0]);
  return 0;
}
