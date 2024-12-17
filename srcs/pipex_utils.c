
// find the path of the command like ls => /usr/bin/ls  cuz the execve needs the path of the cmd to execute it 
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
// executing the path 
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
}