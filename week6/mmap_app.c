#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int main(int argc, char **argv)
{
	int fd;
	fd = open("/dev/mmap", O_RDWR);
	if(fd < 0)
	{
		perror("file open err");
		return -1;
	}

	char *address = NULL;
	//mmap(*state, length, prot, flag, fd, offset)
	//state: 할당받기 원하는 메모리 주소, 0 입력하면 커널이 알아서
	//length: 메모리 맵 할당 크기
	//prot_exec: 해당 페이지 실행 가능, prot_read, write, none..., 이때 매핑할 fd와 속성이 같아야 한다.
	//map_shared는 공유, private은 나만 볼 수 있고 복사해서 쓰는형식(변경된 메모리 속성이 실제 파일에는 반영되지 않음)
	//fd는 메모리 맵방식을 사용할 fd
	//offset은 해당 fd에서 메모리 맵을 시작할 오프셋 값
	address = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(address == MAP_FAILED)
	{
		perror("mmap error");
		return -1;
	}

	printf("initial msg: %s\n", address);
	memcpy(address + 11, " user ", 6);
	//원래 hello from user인데, 메모리에 직접 접근해서 kernel 을 user로 바꿔버림
	printf("changed msg: %s\n", address);
	close(fd);
	return 0;
}
