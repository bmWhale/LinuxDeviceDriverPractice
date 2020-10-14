#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


struct input_event {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	int value;
};

int main(int argc, char *argv[])
{
	int fd_key = -1;
	struct input_event key_evt;
	int ret = 0;

	if(argc<2 || argv[1]==NULL) {
		printf("example: key_read \"/dev/input/event8\"");
		return 0;
	}
	printf("open %s to read key event\n", argv[1]);
	fd_key= open(argv[1], O_RDONLY);

	if(fd_key< 0) {
                printf("open %s error\n", argv[1]);
		return fd_key;
	}

monitor_key:

	ret = read(fd_key, (char*)&key_evt, sizeof(struct input_event));
	if(ret < 0) {
		printf("read key event failed:%d\n", ret);
	} else {
		if(key_evt.code == 408 && key_evt.value == 1) {
			printf("The press of reset button is detected\n");
			return 0;
		}
#if 1
		//printf("%ld seconds %ld microseconds", key_evt.time.tv_sec,
		//       key_evt.time.tv_usec);
                if(key_evt.type!=0)
		        printf("type:0x%x code:0x%x value:0x%x\n", key_evt.type, key_evt.code, key_evt.value);
#endif
	}
	goto monitor_key;
	return 0;

}
