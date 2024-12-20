#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef struct s_pipex
{
    int infile;
    int outfile;
    int (*pipefd)[2];
    char **cmd_args;
    char **env;       
    int cmd_num;
    int pipes_num;
    int argc;
} t_pipex;


char *ft_strnstr(const char *big, const char *little, size_t n);
char **ft_split(char const *s, char c);
char *ft_strjoin(char const *s1, char const *s2);
void *ft_memmove(void *dest, const void *src, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);
size_t ft_strlen(const char *s);
void ft_free(char **get_free);
void exec_path(char *full_cmd, char **env);
char *find_path(char *cmd, char **envp);
void ft_free(char **get_free);
char	*ft_strdup(const char *s1);


#endif 

