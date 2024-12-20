#include "herdoc.h"



// next line to read from terminal => STDIN line by line 
// ./pipex here_doc LIMITER cmd cmd1 file
// ./pipex "here_doc" "EOF" "ls" "wc" outfile
// here_doc> hello\n
//           01234 5
// here_doc> "world    \n"
// here_doc> eof   <- this needs to be the in the first caracter and alone in a line
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
	{
		if (i < (n - 1))
			i++;
		else
			return (0);
	}
	return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	char	ch;

	ch = (char)c;
	while (*s)
	{
		if (*s == ch)
			return ((char *)s);
		s++;
	}
	if (ch == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len1)
		str[j++] = s1[i++];
	i = 0;
	while (i < len2)
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}

char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	size_t			j;
	char			*substr;

	i = -1;
	j = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len >= ft_strlen(s))
		substr = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	else
		substr = (char *)malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	while (s[++i])
	{
		if (i >= start && j < len)
			substr[j++] = s[i];
	}
	substr[j] = '\0';
	return (substr);
}
char	*next_line(char *buffer)
{
	char	*new_static_buffer;
	int		i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i] || !buffer[i + 1])
	{
		free(buffer);
		return (NULL);
	}
	new_static_buffer = ft_substr(buffer, i + 1, ft_strlen(buffer) - i);
	free(buffer);
	return (new_static_buffer);
}

char	*extract_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		line = ft_substr(buffer, 0, i + 1);
	else
		line = ft_strdup(buffer);
	return (line);
}

char	*allocate_buffer(void)
{
	char	*buffer;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	return (buffer);
}

char	*read_from_file(int fd, char *buffer, char *static_buffer)
{
	char	*tmp;
	int		bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!static_buffer)
			static_buffer = ft_strdup("");
		tmp = ft_strjoin(static_buffer, buffer);
		free(static_buffer);
		static_buffer = tmp;
		if (ft_strchr(static_buffer, '\n'))
			break ;
	}
	if (bytes_read < 0 || (!static_buffer && bytes_read == 0))
	{
		free(static_buffer);
		return (NULL);
	}
	return (static_buffer);
}

// char	*get_next_line(int fd)
// {
// 	static char	*static_buffer;
// 	char		*buffer;
// 	char		*line;

// 	if (fd < 0 || BUFFER_SIZE <= 0)
// 		return (NULL);
// 	buffer = allocate_buffer();
// 	if (!buffer)
// 		return (NULL);
// 	static_buffer = read_from_file(fd, buffer, static_buffer);
// 	free(buffer);
// 	if (!static_buffer)
// 		return (NULL);
// 	line = extract_line(static_buffer);
// 	static_buffer = next_line(static_buffer);
// 	return (line);
// }