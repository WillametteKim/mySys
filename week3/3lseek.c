#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

/*
file I/O에 있어
파일 시작점 + offset = relative distance
이러한 relative distance를 이동시킬 수 있는게 lseek()

off_t lseek(int fd, off_t offset, int whence);
									  whence SEEK_SET: based on 시작지점
									  		 SEEK_CUR: based on 현재 위치
									  		 SEEK_END: based on EOF
retval is -1
EBADF: fd is not open fd
ESPIPE: fd is associated with PIPE, socket
EINVAL: whence is not appropriate value
*/

/*
1234567890 이 temp.txt일 때

<END +2>
1234567890
NULL NULL a

<END +0>
1234567890
a						//일단 new line은 기본으로 먹고 간다.

<END -1>
1234567890a

<SET -5>
ERR (lseek retval = -1)

<END -5>
123456a890	//7을 지우고 a 삽입.

*/

int main(int argc, char **argv)
{
	int fd = 0;
	if(argc != 2)
	{
		fprintf(stderr, "err, excution as follows %s<filename>\n", argv[0]);
		return 1;
	}

	fd = open(argv[1], O_RDWR|O_CREAT, 0777);
	if(fd == -1)
	{
		fprintf(stderr, "fail to open %s\n", argv[1]);
		return 1;
	}

	if(lseek(fd, 2, SEEK_END) == -1)
	{
		printf("dont be able to use lseek about %s file\n", argv[1]);
	}
	else
	{
		char insert = 'a';
		write(fd, &insert, 1);
	}

	close(fd);
	return 0;
}
