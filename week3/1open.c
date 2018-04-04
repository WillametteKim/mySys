#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/*
int open(*pathname, int flags);
int opne(*pathname, int flags, mode_t mode);
retval == -1 when fail

두번째 파라미터
O_RDONLY: Read only
O_WRONLY: Write only
O_RDWD	: R+W

세번째 파라미터
O_APPEND
O_TRUNC: 기존 파일 삭제
O_CREAT
O_NONBLOCK
O_SYNC
O_EXCL: 파일 있는지 없는지 확인, CREAT|EXCL 할 경우 파일 존재 시 -1 리턴하고 파일 보존 가능
*/


int main(int argc, char **argv)
{
	int fd = 0;
	if(argc != 3)
	//argv[0]: 실행파일
	//argv[1]: open() 첫번째 파라미터, 여기선 만들 파일이름
	//argv[2]: open() 세번째 파라미터, octet num like 0777
	{
		fprintf(stderr, "usage: %s <file name> <access mode>\n", argv[0]);
		fprintf(stderr, "access mode is octet num\n");
		return 1;
	}

	mode_t mode = 0;
	sscanf(argv[2], "0%o", &mode);
	fd = open(argv[1], O_WRONLY|O_CREAT|O_EXCL, mode);
	if(fd == -1)
	{
		perror("failed open");
		return 1;
	}

	printf("success open %s\n", argv[1]);
	close(fd);
	return 0;
}
