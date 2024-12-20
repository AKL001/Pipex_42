#include "pipex.h"

static void failed_to(void)
{
    perror("\033[31mError");
    exit(EXIT_FAILURE);
}

void get_init(t_pipex *pipex, char *infile, char *cmds[], int argc)
{
    int i;

    pipex->infile = open(infile, O_RDONLY, 0777);
    if (pipex->infile == -1)
    {
        close(pipex->infile);
        failed_to();
    }
    pipex->pipefd = malloc(sizeof(*pipex->pipefd) * pipex->pipes_num);
    if (!pipex->pipefd)
        return;
    pipex->cmd_args = malloc(sizeof(char *) * (pipex->cmd_num + 1));
    if (!pipex->cmd_args)
        return;
    pipex->cmd_args[pipex->cmd_num] = NULL;
    i = -1;
    while (++i < pipex->cmd_num)
    {
        pipex->cmd_args[i] = ft_strdup(cmds[i]);
        if (!pipex->cmd_args[i])
        {
            ft_free(pipex->cmd_args);
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
void exec_childs(int i,t_pipex *pipex)
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
    close_all_pipes(pipex); 
    exec_path(pipex->cmd_args[i], pipex->env);
}
void execute_all_cmds(t_pipex *pipex)
{
    int i,pids;

    i = -1;
    while (++i < pipex->cmd_num)
    {
        pids = fork();
        if (pids == -1)
            failed_to();
        if (pids == 0)
        {
            exec_childs(i,pipex);
            ft_free(pipex->cmd_args);
            free(pipex->pipefd);
            failed_to();
        }
    }
    close_all_pipes(pipex);
    i = -1;
    while (++i < pipex->cmd_num)
        wait(NULL);
}

int main(int argc, char *argv[], char *env[])
{
    t_pipex pipex;
    if (argc >= 5)
    {
        pipex.env = env;
        pipex.cmd_num = argc - 3;
        pipex.pipes_num = pipex.cmd_num - 1;
        pipex.argc = argc;
        // if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		// {
		// 	pipex.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		// 	here_doc(argv[2], argc);
		// }
        // else
        // {
        get_init(&pipex, argv[1], &argv[2], argc);
        pipex.outfile = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (pipex.outfile == -1)
            failed_to();
        create_pipes(&pipex);
        execute_all_cmds(&pipex);
        ft_free(pipex.cmd_args);  
        free(pipex.pipefd);
        // }
    }
    else
        failed_to();
}
