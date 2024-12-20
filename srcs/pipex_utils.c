#include "../includes/pipex.h"

char	*ft_strnstr(const char *big, const char *little, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < n)
	{
		if (big[i] == little[j])
		{
			while (big[i + j] == little[j] && i + j < n)
			{
				if (little[j + 1] == '\0')
					return ((char *)big + i);
				j++;
			}
			j = 0;
		}
		i++;
	}
	return (NULL);
}

void failed_to(void)
{
    perror("\033[31mError");
    exit(EXIT_FAILURE);
}

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*dst;

	len = ft_strlen(s1);
	dst = (char *)malloc(sizeof(char) * (len + 1));
	if (!dst)
		return (0);
	ft_memcpy(dst, s1, len);
	dst[len] = '\0';
	return (dst);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		len = 0;
	else if (len > s_len - start || len > s_len)
		len = s_len - start;
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (ptr == NULL)
		return (NULL);
	ft_memcpy(ptr, s + start, len);
	ptr[len] = '\0';
	return (ptr);
}

 int	count_words(char const *s, char c)
{
	size_t	counter;
	size_t	i;
	int		flag;

	flag = 1;
	i = 0;
	counter = 0;
	while (s[i])
	{
		if (s[i] != c && flag)
		{
			counter++;
			flag = 0;
		}
		else if (s[i] == c)
			flag = 1;
		i++;
	}
	return (counter);
}

 int	free_allocate(char **ptr, int pos)
{
	while (pos--)
	{
		free(ptr[pos]);
	}
	free(ptr);
	return (1);
}

int	allocate_words(char **ptr, char *s, char c)
{
	unsigned int	i;
	size_t			len;
	char			*word;

	i = 0;
	while (*s)
	{
		len = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			len++;
			s++;
		}
		if (len > 0)
		{
			word = ft_substr(s - len, 0, len);
			if (!word)
				return (free_allocate(ptr, i));
			ptr[i++] = word;
		}
	}
	ptr[i] = NULL;
	return (0);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**ptr;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	ptr = (char **)malloc(sizeof(char *) * (words + 1));
	if (!ptr)
		return (NULL);
	ptr[words] = NULL;
	if (allocate_words(ptr, (char *)s, c))
		return (NULL);
	return (ptr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	max_len;
	char	*ptr;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	max_len = s1_len + s2_len;
	ptr = (char *)malloc(sizeof(char) * (max_len + 1));
	if (ptr == NULL)
		return (NULL);
	ft_memmove(ptr, s1, s1_len);
	ft_memmove(ptr + s1_len, s2, s2_len);
	ptr[max_len] = '\0';
	return (ptr);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (dest == src)
		return (dest);
	if (!dest && !src)
		return (NULL);
	if (dest < src)
		ft_memcpy(dest, src, n);
	else
	{
		while (n--)
		{
			*((unsigned char *)(dest + n)) = *((unsigned char *)(src + n));
		}
	}
	return (dest);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (dest == src)
		return (NULL);
	if (!dest && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		*((unsigned char *)(dest + i)) = *((unsigned char *)(src + i));
		i++;
	}
	return (dest);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}
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
    if (!part_path)
      break;
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			break ;
    free(path);
    path = NULL;
	}
	ft_free(paths);
	return (path);
}
// executing the path 
void exec_path(char *full_cmd, char **env)
{
	char **cmd;
	char *path;
	int i; 

	cmd = ft_split(full_cmd, ' ');
    if (!cmd)
        return;
	path = find_path(cmd[0], env);
	if (!path)
	{
    i = -1;
		while(cmd[++i])
			free(cmd[i]);
		free(cmd);
		return;
	}
	if (execve(path,cmd,env) == -1)
	{
		free(path);
		ft_free(cmd);
	}
}
void ft_free(char **get_free)
{
	int i;

	i = 0;
	while(get_free[i])
	{
		free(get_free[i]);
		i++;
	}
	free(get_free);
	return;
}
// find the path of the command like ls => /usr/bin/ls  cuz the execve needs the path of the cmd to execute it 
// executing the path 
