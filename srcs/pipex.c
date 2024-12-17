#include "../includes/pipex.h"
/*
void exec_path(char *full_cmd, char **env);
char	*find_path(char *cmd, char **envp);
*/

void exec_child(char **av,char **env,int *fd)
{
  int file;

  file = open(av[1], O_RDONLY, 0777);
  if (file == -1)
    return ;
  dup2(file, STDIN_FILENO);
  dup2(fd[1], STDOUT_FILENO);
  close(fd[0]);
  close(fd[1]);
  close(file);
  exec_path(av[2],env);
}

void exec_parent(char **av,char **env,int *fd)
{
  int file;

  file = open(av[4], O_WRONLY | O_CREAT , 0777);
  if (file == -1)
    return ;
  dup2(file, STDOUT_FILENO);
  dup2(fd[0],STDIN_FILENO);
  close(fd[1]);
  close(fd[0]);
  close(file);
  exec_path(av[3],env);
}

int main(int ac,char *av[],char *env[])
{
  
  if (ac == 5)
  {
    int pid;
    int fd[2];

    if (pipe(fd) == -1)
    {
      perror("pipe");
      return 2;
    }
    pid = fork();
    if (pid == -1)
    {
      perror("fork");
      return 2;
    }
    if (pid == 0)
      exec_child(av, env, fd);
    waitpid(pid, NULL,0);
    exec_parent(av, env, fd);
    close(fd[0]);
    close(fd[1]);
  }
  else {
    printf("less or more arguments\n");
  }
}
