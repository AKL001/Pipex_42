#include "libft/libft.h"
#include <stdio.h>

// 25 lines no leaks
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

int main(int ac, char *av[], char *env[])
{
    char *cmd = find_path(av[1], env);  // Find the path for the given command
	
    printf("%s\n", cmd);  // Print the found path
    free(cmd);  // Free the dynamically allocated path after use

    return 0;
}
