#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>

/*
int dup(int oldfd);
int dup2(int oldfd, int newfd);

-oldfd의 file descriptor를 복제(공유) -> share Lock, Floags, File Location Pointers...
-복제된 새로운 fd는 현재 사용되고 있지 않은 file specifier 중 가장 작은 수를 던져준다.
-dup2와 dup의 차이는 newfd를파라미터로  지정 가능.
 예를 들어, 기존에 fd1과 fd2를 사용 중이었는데
 dup2(fd1, fd2) 하면 fd2를 닫고fd1을 2로 복제

*/

int main(int argc, char **argv)
{
	int fd1 = 0, fd2 = 0, fd3 = 0;
	char buf[5+1] = "";
	char buf1[6] = "";
	char buf2[6] = "";

	if(argc != 2)
	{
		fprintf(stderr,"err num of parameters: %s <file name>\n", argv[0]);
		return 1;
	}

	fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1)
	{
		fprintf(stderr, "failed to open\n");
		return 1;
	}
	else printf("open fd1: %d\n", fd1);


	fd2 = dup(fd1);
	if(fd2 == -1)
	{
		perror("fail to use dup\n");
		return 1;
	}
	else printf("dup(fd1): %d\n", fd2);

	fd3 = dup2(fd1, 4);
	if(fd3 == -1)
	{
		perror("fail to open\n");
		return 1;
	}
	else printf("dup2(fd1, 3): %d\n", fd3);



	read(fd1, buf, 5);
	printf("at fd%d: %s\n", fd1, buf);
	close(fd1);

	read(fd2, buf1, 5);
	printf("at fd%d: %s\n", fd2, buf1);
//	close(fd2);

	//여기선 fd1 fd2 close한 뒤 fd3로 read하는 것이니, 결국 파일 처음부터 fd1으로 읽는 것과 같다.
	read(fd3, buf2, 5);
	printf("at fd%d: %s\n", fd3, buf2);

	return 0;
}
