#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>

/*
**fcntl: dup2()와 거진 유사하지만, fd가 이미 사용 중일 경우
  새로운 idle descripto를 사용한다.f
**Input Parameter: (int fd, int cmd...)
**Return value:		 if fail, return -1.

F_DUPFD means duplicate fd
F_GETFL means getting access mode of file status flag.
	int value = fcntl(fd, F_GETFL, 0);
*/

int main(int argc, char **argv)
{
	int fd1 = 0, fd2 = 0, fd3 = 0;
	int val;
	char buf[5+1] = "";

	fd1 = open("dup_fcntl_test", O_WRONLY|O_CREAT|O_TRUNC);
	printf("***** TEST dup ( , ) *****\n");

	printf("FD: FD1 = %d\n", fd1);

	fd2 = dup2(fd1, 10);
	printf("FD: FD2 = %d\n", fd2);

	fd3 = dup2(fd1, 10);
	printf("FD: FD3 = %d\n", fd3);

	close(fd2);
	close(fd3);

	printf("**** TEST fcntl ( , F_DUPPED, ) ****\n");

	fd2 = fcntl(fd1, F_DUPFD, 10);
	printf("FD 2: %d\n", fd2);

	fd3 = fcntl(fd1, F_DUPFD, 10);
	printf("FD 3: %d\n", fd3);

	val = fcntl(fd3, F_GETFL, 0);
	val = val & O_ACCMODE;
	printf("open mode: %d\n", val);

	return 0;
}
