#include "pipex.h"

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

static void failed_to(void)
{
    perror("\033[31mError");
    exit(EXIT_FAILURE);
}

void get_init(t_pipex *pipex, char *infile, char *cmds[], int argc)
{
    pipex->infile = open(infile, O_RDONLY);
    if (pipex->infile == -1)
        failed_to();

    pipex->pipefd = malloc(sizeof(*pipex->pipefd) * pipex->pipes_num);
    if (!pipex->pipefd)
        failed_to();

    pipex->cmd_args = malloc(sizeof(char *) * (pipex->cmd_num + 1));
    if (!pipex->cmd_args)
    {
        free(pipex->pipefd);
        failed_to();
    }
    pipex->cmd_args[pipex->cmd_num] = NULL;

    int i = -1;
    while (++i < pipex->cmd_num)
    {
        pipex->cmd_args[i] = ft_strdup(cmds[i]);
        if (!pipex->cmd_args[i])
        {
            ft_free(pipex->cmd_args);
            free(pipex->pipefd);// what ???
            failed_to();
        }
    }
}

void create_pipes(t_pipex *pipex)
{
    int i = -1;
    while (++i < pipex->pipes_num)
    {
        if (pipe(pipex->pipefd[i]) == -1)
        {
            while (--i >= 0)   // ??????? 
            {
                close(pipex->pipefd[i][0]);
                close(pipex->pipefd[i][1]);
            }
            free(pipex->pipefd);// ok ok 
            ft_free(pipex->cmd_args);
            failed_to();
        }
    }
}

void close_all_pipes(t_pipex *pipex)
{
    int j = -1;
    while (++j < pipex->pipes_num)
    {
        close(pipex->pipefd[j][0]);
        close(pipex->pipefd[j][1]);
    }
}

void execute_all_cmds(t_pipex *pipex)
{
    int *pids = malloc(sizeof(pid_t) * pipex->cmd_num);
    if (!pids)
        failed_to();

    int i = -1;
    while (++i < pipex->cmd_num)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            free(pids);
            failed_to();
        }
        if (pids[i] == 0)
        {
            if (i == 0)
            {
                dup2(pipex->infile, STDIN_FILENO);
                close(pipex->infile);
            }
            else
                dup2(pipex->pipefd[i-1][0], STDIN_FILENO);

            if (i == pipex->cmd_num - 1)
            {
                dup2(pipex->outfile, STDOUT_FILENO);
                close(pipex->outfile);
            }
            else if (i != pipex->cmd_num - 1)
                dup2(pipex->pipefd[i][1], STDOUT_FILENO);
            // If it's the last command and no outfile, keep STDOUT as is

            close_all_pipes(pipex);
            exec_path(pipex->cmd_args[i], pipex->env);
            exit(EXIT_FAILURE);
        }
    }

    close_all_pipes(pipex);
    
    i = -1;
    while (++i < pipex->cmd_num)
        waitpid(pids[i], NULL, 0);
    
    free(pids);
}

int main(int argc, char *argv[], char *env[])
{
    if (argc < 5)
    {
        fprintf(stderr, "Not enough arguments!\n");
        return EXIT_FAILURE;
    }

    t_pipex pipex;
    pipex.env = env;
    pipex.cmd_num = argc - 3;
    pipex.pipes_num = pipex.cmd_num - 1;
    pipex.argc = argc;

    get_init(&pipex, argv[1], &argv[2], argc);
    
    // Only try to open outfile if it's provided
    pipex.outfile = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (pipex.outfile == -1)
      failed_to();
    create_pipes(&pipex);
    execute_all_cmds(&pipex);

    ft_free(pipex.cmd_args);
    free(pipex.pipefd);
    
    return EXIT_SUCCESS;
}
