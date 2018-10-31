#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define BUFLEN 256

#define MY_IOCTL_NUMBER 100
#define MY_IOCTL_READ		  _IOR( MY_IOCTL_NUMBER, 0, int )
#define MY_IOCTL_WRITE		_IOW( MY_IOCTL_NUMBER, 1, int )
#define MY_IOCTL_STATUS		_IO(  MY_IOCTL_NUMBER, 2 )
#define MY_IOCTL_READ_WRITE	_IOWR(MY_IOCTL_NUMBER, 3, int)
#define MY_IOCTL_NR		4

int main()
{
  int ret, fd;
  char sendMsg[4];
  char rcvMsg[8];
  fd = open("/dev/ioctl_app_dev", O_RDWR);

  if(fd < 0)
  {
    perror("failed to open the device\n");
      return errno;
  }

  printf("Type short string send to kernel:\n");
  scanf("%s", sendMsg);
  printf("Writing msg to device [%s].\n", sendMsg);

	/*
	-read(), write()와 달리 디바이스의 복잡한 기능을 수행하거나 상태를 얻기 위해 사용
	-왜?  file_operations 구조체에 존재하는 함수의 갯수에는 제한이 있으므로, 여기에 적혀있는것 외에 다른 함수를
 	 사용할 경우엔 ioctl 사용하는 것이 적절하다.
	-Unique function for each Device!
	ioctl(fd, REQUEST(요청코드 매크로), 보조인자)
	return 0 if success, -1 if fail
	*/
  ioctl(fd, MY_IOCTL_WRITE, sendMsg);

  printf("Reading from device..\n");

  char *temp;
  ioctl(fd, MY_IOCTL_READ, rcvMsg);
  printf("Rcv msg is: [%s]\n", rcvMsg);
  printf("GOOD BYE-------------- \n");

  return 0;
}
