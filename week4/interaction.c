//로컬 변수 버젼
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

void writeOperation();
void readOperation();

int main()
{
	while(1)
	{
		writeOperation();
		readOperation();
	}
	return 0;
}

void writeOperation()
{
	int fd = 0;
	int wd = 0;	//number of written bytes
	char buffer[MAX_LEN];

	fd = open(TEMP_FILE_NAME, O_WRONLY, MODE);
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
	//else printf("success %d bytes\n", wd);
	close(fd);
}

void readOperation()
{
	char buffer[MAX_LEN];
	int fd = 0;

	fd = open(TEMP_FILE_NAME, O_RDONLY);
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
