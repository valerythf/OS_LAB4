#include <stdio_ext.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#define MAX_SIZE 100
#define SEGSIZE 100

int main() {
	int  size;
	char str[MAX_SIZE];
	key_t key;                 // целое (по меньшей мере 32-разрядное).
	int shmid;
	char* segptr;

	key = ftok(".", 'S');         // файл и идентификатор проекта в ключ для системных вызовов  
	printf("Введите стороку(максимум 100 символов)\n");
	__fpurge(stdin);
	fgets(str, MAX_SIZE, stdin);
	fflush(stdout);
	size = strlen(str);
	char* str1[] = { str, NULL };

	int child1 = fork(), child2;

	if (child1 < 0) {
		perror("error");
		exit(1);
	}
	else if (child1 >= 0) {
		child2 = fork();
		if (child2 < 0) {
			perror("error");
			exit(1);
		}
		else if (child1 > 0 && child2 > 0) {
			printf("It's a parent, child1 id: [%d], child2 id: [%d]\n", child1, child2);
			if ((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0660)) == -1)   //возвращает идентификатор разделяемому сегменту памяти, соответствующий значению аргумента key. Создается новый разделяемый сегмент памяти с размером size. IPC_CREAT - создает сегмент, если его нет
			{
				printf("Shared memory segment exists - opening as a client\n");
				if ((shmid = shmget(key, SEGSIZE, 0)) == -1)
				{
					perror("shmget");
					exit(1);
				}
				else {
					printf("Creating new shared memory segment\n");
				}
			}
			if ((segptr = shmat(shmid, 0, 0)) == -1) //подстыковывает сегмент разделяемой памяти shmid к адресному пространству вызывающего процесса
			{
				perror("shmat");
				exit(1);
			}
			waitpid(-1, NULL, 0);
			sleep(1);
			printf(" result %s ", segptr);
			shmdt(segptr);
			shmctl(shmid, IPC_RMID, 0);
		}
		else if (!child1 && child2) {
			if (size > 10) { exit(1); }
			execv("./child", str1);
		}
		else if (child1 && !child2) {
			if (size <= 10) { exit(1); }
			execv("./child", str1);
		}
	}
	return 0;
}
