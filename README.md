# Pipex_42
42_pipex is a 42 project that recreates the shell's pipe (|) functionality. 

 # To DO 
the logic behind this project to know how to redirect outputs of programs/scripts to another ones

-> include <unistd.h> // for pipe(), read() , write();


--system calls etc
fork(); function creat a child for each  parent process .
if we have one process we creat a child process for it in the first call 
so the parent have one  child process, in the second call of fork(); we getwe creat childs for each process which means the first child would have a child process and the parent process would have another child process;
--
fork(); called 2 times:
    parent
    |-first child => first fork();
    |       |-first child => second fork();
    |-second child => second fork();
--


--Pipes:
    int fd[2];
    pipe(fd);
    |- this means the program created a pipe 
    |- fd[1] ========= fd[0];
    fd[1] => write(); writing into pipe;
    fd[0] => read (); reading from pipe;

-> at the end of the program you need to close both fd[0] & fd[1];
->



