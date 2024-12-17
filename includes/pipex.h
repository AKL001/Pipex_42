#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/libft.h"


char	*find_path(char *cmd, char **envp);
void exec_path(char *full_cmd, char **env);


#endif 
