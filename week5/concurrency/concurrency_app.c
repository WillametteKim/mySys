#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int i, fd;
  char ch, writeBuf[100], readBuf[100];

  printf("open file\n");
  fd = open("/dev/ioctl_app_dev", O_RDWR);

  if(fd == -1)
  {
    printf("file open err\n");
    return -1;
  }

  printf("please enter op.W?R: ");
  scanf("%c", &ch);
  getchar();

  switch(ch)
  {
    case 'w':
     printf("enter data: ");
     scanf("%[^\n]", writeBuf);
     write(fd, writeBuf, sizeof(writeBuf));
     break;

    case 'r':
     read(fd, readBuf, sizeof(readBuf));
     printf("recv is %s\n", readBuf);
     break;
  }
  close(fd);
  return 0;
}
