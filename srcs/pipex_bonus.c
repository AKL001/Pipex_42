#include "../includes/pipex.h"

int main(int ac, char *av[], char **env) {
    int fd[ac - 4][2];  // n-1 pipes for n commands
    int i, j;

    // Create pipes
    i = 0;
    while (i < ac - 4) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    // Fork and execute commands
    i = 0;
    while (i < ac - 3) {  // ac-3 commands means ac-4 pipes
        pid_t pid = fork();
        
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // In child process
            // Redirect input
            if (i == 0) {  // First command needs to read from input file
                int infile = open(av[1], O_RDONLY, 0644);  // Read from input file
                if (infile < 0) {
                    perror("fail to open input file");
                    exit(EXIT_FAILURE);
                }
                dup2(infile, STDIN_FILENO);
                close(infile);
            } else {  // Intermediate commands need to read from previous pipe
                dup2(fd[i - 1][0], STDIN_FILENO);
            }

            // Redirect output
            if (i == ac - 4) {  // Last command needs to write to output file
                int outfile = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);  // Write to output file
                if (outfile < 0) {
                    perror("fail to open output file");
                    exit(EXIT_FAILURE);
                }
                dup2(outfile, STDOUT_FILENO);
                close(outfile);
            } else {  // Intermediate commands need to write to next pipe
                dup2(fd[i][1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors
            j = 0;
            while (j < ac - 4) {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }

            // Execute the command
            exec_path(av[i + 2], env); 
             // Execute the command passed in the arguments
        }
        i++;
    }
    // Parent process: Close all pipe file descriptors
    j = -1;
    while (++j < ac - 4) {
        close(fd[j][0]);
        close(fd[j][1]);
    }
    // Wait for all child processes
    i = -1;
    while (++i < ac - 3) {
        wait(NULL);
    }

    return 0;
}
