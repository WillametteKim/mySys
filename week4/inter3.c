//메인에서 포인터 버젼
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 32
#define TEMP_FILE_NAME "/dev/simple_chr_dev"
#define MODE 0777

/*
ssize_t read(int fd, void *buff, size_t nbytes);
retval: n if read 'n' bytes, -1 if failure, 0 if EOF

ssize_t write(int fd, void *buff, size_t nbytes);
retval: n if write 'n' bytes, -1 if failure

<PROCEDURE>
open file: if file already exists, flush and write
write file
read file
*/

void writeOperation(char *writtenData);
void readOperation(char *readData);

int main()
{
	char readData[MAX_LEN];
	char writtenData[MAX_LEN];

	while(1)
	{
		memset(readData,		NULL, MAX_LEN);
		memset(writtenData, NULL, MAX_LEN);
		writeOperation(writtenData);
		readOperation(readData);
	}
	return 0;
}

void writeOperation(char *writtenData)
{
	int fd = 0;
	int wd = 0;	//number of written bytes

	fd = open(TEMP_FILE_NAME, O_WRONLY, MODE);
	if(fd == -1)
	{
		perror("fail to open file\n");
		exit(1);
	}

	//get User Input
	printf("WRITE TO DEVICE \t>> ");
	scanf("%s", writtenData);

	//send to Device File
	wd = write(fd, writtenData, MAX_LEN);
	if(wd == -1)
	{
		perror("fail to write content\n");
		exit(1);
	}
	//else printf("success %d bytes\n", wd);
	close(fd);
}


void readOperation(char *readData)
{
	int fd = 0;
	int rd = 0;	//number of read bytes

	//open file descriptor
	fd = open(TEMP_FILE_NAME, O_RDONLY);
	if(fd == -1)
	{
		perror("failed to open file\n");
		exit(1);
	}

	//read from Device File
	rd = read(fd, readData, MAX_LEN);
	if(rd == -1)
	{
		perror("fail to read content\n");
		exit(1);
	}
	else
	{
		printf("READ FROM DEVICE \t<< ");
		printf("%d", rd);
		printf("%s\n", readData);
		printf("************ GOOD BYE ************\n\n");
	}
	close(fd);
}
