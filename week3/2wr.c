#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 100
#define TEMP_FILE_NAME "studentRecord.txt"
#define MODE 0644

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

/*
이 결과로 생긴 파일을 cat으로 보았을 때 이상하게 뜨는 이유는?
->write할 때 그냥 바이너리 형식으로 '구조체 자체를 기록'했기 때문이다.
->이에 대한 반증으로 read 시 따로 형식 안맞춰주고 바로 구조체 형식으로 출력 가능했잖아.
*/

typedef struct student{
	char name[MAX_LEN +1];
	int stu_num;
	int score;
}Student;

void writeOperation();
void readOperation();

int main()
{
	writeOperation();
	readOperation();
	return 0;
}

void writeOperation()
{
	int fd = 0;
	int wd = 0;	//number of written bytes
	Student students[3] = {
		{"John", 1004, 100},
		{"Park", 1005, 75},
		{"Nam", 1006, 50}
	};

	fd = open(TEMP_FILE_NAME, O_WRONLY|O_CREAT|O_EXCL, MODE);
	if(fd == -1)
	{
		perror("fail to open studentRecord file\n");
		exit(1);
	}

	wd = write(fd, students, sizeof(Student) *3);
	if(wd == -1)
	{
		perror("fail to write content\n");
		exit(1);
	}

	printf("success %d bytes\n", wd);
	close(fd);
}

void readOperation()
{
	int fd = 0;
	Student student;

	fd = open(TEMP_FILE_NAME, O_RDONLY);
	if(fd == -1)
	{
		perror("failed to open file\n");
		exit(1);
	}

	int rd = 0;	//number of read bytes
	while((rd = read(fd, &student, sizeof(Student))) > 0)
	{
		printf("student name	:%s\n", student.name);
		printf("student number	:%d\n", student.stu_num);
		printf("student score	:%d\n", student.score);
	}

	close(fd);
}
