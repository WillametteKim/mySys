#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 32
#define DEV_FILE_NAME "/dev/simple_chr_dev"		//빨대 꽂을 디바이스 파일
#define MODE 0777

static char rcvMsg[BUFSIZE];

int main()
{
	int retval, fd;
	char sendMsg[BUFSIZE];

	//open file
	fd = open(DEV_FILE_NAME, O_RDWR, MODE);
	if(fd == -1)
	{
		perror("fail to open file\n");
		exit(1);
	}

	//get msg from usr
	printf("Type in a short string to send to KERNEL module(MAX LEN=32, Ignore Case):\n");
	scanf("%[^\n]", sendMsg);

	//write to file
	printf("Writing msg to device: \t[%s].\n", sendMsg);
	retval = write(fd, sendMsg, BUFSIZE);
	if(retval < 0)
	{
		perror("fail to write content\n");
		exit(1);
	}

	//read from file
	retval = read(fd, rcvMsg, BUFSIZE);
	if(retval < 0)
	{
		perror("fail to read content\n");
		exit(1);
	}
	else printf("Received msg is: \t[%s].\n", rcvMsg);

	printf("*******GOOD BYE********\n\n");
	close(fd);

	return 0;
}
