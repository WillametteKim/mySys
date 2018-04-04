//전역변수 버젼
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 32
#define TEMP_FILE_NAME "/dev/simple_chr_dev"
#define MODE 0777

static char buffer[MAX_LEN];

void writeOperation();
void readOperation();

int main()
{
	while(1)
	{
		memset(buffer, NULL, MAX_LEN);
		writeOperation();
		memset(buffer, NULL, MAX_LEN);
		readOperation();
	}
	return 0;
}

void writeOperation()
{
	int fd = 0;
	int wd = 0;	//number of written bytes
	//char buffer[MAX_LEN];

	fd = open(TEMP_FILE_NAME, O_RDWR, MODE);
	if(fd == -1)
	{
		perror("fail to open file\n");
		exit(1);
	}

	//get User Input
	printf("WRITE TO DEVICE \t>> ");
	scanf("%s", buffer);

	//send to Device File
	wd = write(fd, buffer, MAX_LEN);
	if(wd == -1)
	{
		perror("fail to write content\n");
		exit(1);
	}
	else printf("%s success %d bytes\n", buffer, wd);
	close(fd);
}

void readOperation()
{
	//char buffer[MAX_LEN];
	int fd = 0;

	fd = open(TEMP_FILE_NAME, O_RDWR);
	if(fd == -1)
	{
		perror("failed to open file\n");
		exit(1);
	}

	//read from Device File
	int rd = 0;	//number of read bytes
	if(rd = read(fd, *buffer, MAX_LEN) < 0)
		perror("fail to read content\n");
	else
	{
		printf("READ FROM DEVICE \t<< ");
		printf("%s\n", buffer);
		printf("************ GOOD BYE ************\n\n");
	}
	close(fd);
}
