// so this here_doc will read from STDIN and let say write in a buffer then gonna
// here_doc stops if delimiter 
// so i should read line by line 
/*
    cmd << LIMITER | cmd1 >> file
 ./pipex here_doc LIMITER cmd cmd1 file
    0       1        2     3   4    5
*/
// ./a.out here_doc limiter cmd1 gonna read from the "buffer" 
// 0- we gonna read from stdin till we reach end of file and the 
// cmd << limiter | we gonna pass what we read from stdin to cmd
// so cmd gonna read from the buffer 
// and output that to cmd2 using pipe and then
// append that in a file
// 1- buffer[] => hello\n world \n last '\0'
// 2- cmd1 read from 

// so first  we gonna exec_cmd(av[3]);
//
// get_next_line;

//  ./pipex here_doc LIMITER cmd cmd1 file
// cmd << eof
// eof
#include "herdoc.h"

typedef struct s_here_doc
{
    int outfile;
    char *buffer;
} t_here_doc;

int get_next_line(char *delimiter,t_here_doc *here_doc)
{
    char *buffer;
    int bytes_read;
    char *tmp;
    char c;
    int i;

    buffer = malloc(1000 * sizeof(char));
    if (!buffer)
        return 2;
    bytes_read = read(0, &c,1);
    i = 0;
    while (bytes_read > 0)
    {
        if (c != '\n' && c)  
            buffer[i++] = c;
        if (c == '\n')
        {
            buffer[i++] = c;
            buffer[i] = '\0';
            break; // Save the newline character
        }
        bytes_read = read(0, &c,1);
    }
    if (ft_strncmp(buffer,delimiter,ft_strlen(delimiter)) == 0)
    {
        free(buffer);
        return 0;
    }
    else
    {
        tmp = ft_strjoin(here_doc->buffer,buffer);
        if (!tmp)
        {
            free(buffer);
            return 2;
        }
        here_doc->buffer = tmp;
        tmp = NULL; 
        free(tmp);
        free(buffer);
        return 1;  
    }
}
int main(int agrc,char *argv[],char *env[])
{

    t_here_doc here_doc;
    here_doc.buffer = ft_strdup("");
    if (!here_doc.buffer)
        return 0;
    if (ft_strncmp(argv[1],"here_doc",8) == 0)
    {
        write(1, "here_doc>",9);
        while (get_next_line(argv[2],&here_doc) == 1)
        {
            write(1, "here_doc>",9);
        }
        // write(1, &here_doc.buffer,ft_strlen(here_doc.buffer));
        printf("%s\n",here_doc.buffer);
        free(here_doc.buffer);
      
      //write(1, "yes\n",4);
  }
}
