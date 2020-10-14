#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>    
int main()
{
    char bytes[10];
    int fd = open("/dev/example", O_RDWR);
	ssize_t size=0;
    size = write(fd, "hello", strlen("hello"));
	printf("write byte[%d]hello\n",size);
    size = read(fd, &bytes, sizeof(bytes));
    printf("Read byte[%d]%s\n", size, bytes);
	close(fd);
    return 0;
}
