#include "pipex.h"

typedef struct s_pipex
{
    int infile;         // Input file descriptor
    int outfile;        // Output file descriptor
    int *pipefd[2];      // Pipe file descriptors
    char **cmd_args;
    char **env;       
    int cmd_num;
    int pipes_num;
    int argc;
} t_pipex;

static void	failed_to(void)
{
	perror("\033[31mError");
	exit(EXIT_FAILURE);
}
void ft_free(char **get_free)
{
	int i;

	i = -1;
	while(get_free[++i])
		free(get_free[i]);
	free(get_free);
	return;
}
void get_init(t_pipex *pipex,char *infile,char *cmds[],int argc)
{
  int i;

  pipex->infile = open(infile , O_RDONLY, 0777);
  if (!(pipex->infile) == -1)
    failed_to();
  pipex->pipefd[2] = malloc(sizeof(int *) * pipex->pipes_num);
  if (!pipex->pipefd)
    return;
  //
  pipex->cmd_args = (char **)malloc(sizeof(char *) * (pipex->cmd_num + 1));
  if (!pipex->cmd_args)
    return ;
  pipex->cmd_args[pipex->cmd_num] = NULL;
  // all cmds allocated 
  i = 0;
  while (i < pipex->cmd_num)
  {
    pipex->cmd_args[i] = ft_strdup(cmds[i+2]);
    i++;
  }
  //
}
void create_pipes(int ac,t_pipex *pipex)
{
  int i;
  int fd[pipex->pipes_num][2];

  while (i < pipex->pipes_num)
  {
    if (pipe(fd[i]) == -1)
      failed_to();
    i++;
  }
}

void close_all_pipes(t_pipex *pipex)
{
  int j;

  j = -1;
  while (++j < pipex->pipes_num)
  {
    close(pipex->pipefd[j][0]);
    close(pipex->pipefd[j][1]);
  }
}
void execute_all_cmds(t_pipex *pipex)
{
  int i,pid;

  i = 0;
  while(i < pipex->cmd_num)
  {
    pid = fork();

    if (pid == 0)
    {
      if (i == 0)
      {
        dup2(pipex->infile, STDIN_FILENO);
        close(pipex->infile);
      }
      else
        dup2(pipex->pipefd[i-1][0], STDIN_FILENO); // this if its not the first command so it should read from the last "cmd PIPE"

      // redirect output file
      // if we are in the last cmd we should redirect the output to the file else we redirect it to the next cmd
      if (i == pipex->cmd_num-1)
      {
        if(pipex->outfile == -1)
          dup2(STDOUT_FILENO, STDOUT_FILENO);
        else
        {
        dup2(pipex->outfile, STDOUT_FILENO);
        close(pipex->outfile);
        }
      }
      else
        dup2(pipex->pipefd[i][1],STDOUT_FILENO);
      close_all_pipes(pipex);
      exec_path(pipex->cmd_args[i],pipex->env);
    } 
    i++;
  }
  close_all_pipes(pipex);
  i = -1;
  while (++i < pipex->cmd_num)
    wait(NULL);
}
int  main(int argc, char *argv[],char *env[])
{
  /*char *cmd1[] = {"cat" , "-e",NULL};
  char *cmd2[] = {"grep", "hello",NULL};
  char *cmd3[] = {"sort",NULL};
  */

  // ./a.out infile "cat -e" "grep hello" "sort" outfile
  //    1      2      3           4         5       6
  //  3 cmds => 2 pipes  ; 3 forks; 2 pipes
  
  if(argc >= 5)
  {
    t_pipex pipex;
    pipex.env = env;
    pipex.cmd_num =  argc-3;
    pipex.pipes_num = argc-4;
    pipex.argc = argc;
    //int num_cmds = argc - 3;
    //       struct  infile  first cmd , from the second command to the last ones 
    get_init(&pipex,argv[1],&argv[3],argc);
    
    create_pipes(argc,&pipex);
    // after creating pipes we need to create the forks for each cmd so its gonna be ac - 3
    pipex.outfile = open(argv[argc-1],  O_WRONLY | O_CREAT | O_TRUNC , 0777);
    if ((pipex.outfile) == -1)
      failed_to();
    execute_all_cmds(&pipex);
    ft_free(pipex->cmd_args); 
  }
  else
  {
        fprintf(stderr, "Not enough arguments!\n");
        return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
