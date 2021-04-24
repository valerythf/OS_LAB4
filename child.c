#include <stdio_ext.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include<sys/wait.h>
#include <stdio.h>
#define MAX_SIZE 100
#define SEGSIZE 100

char Inversion(char* string ,int i){
  char str;
  int n = 0;
  int k = i/2;
while(i != k){
  str = string[n];
  string[n] = string[i-1];
  string[i-1] = str;
  i--;
  n++;
}
}

int main(int argc, char* argv[]) {
  char* str[100];
  key_t key;
   int shmid, cntr;
   char *segptr;
    key = ftok(".", 'S');
   if((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0660)) == -1)
   {
      printf("Shared memory segment exists - opening as a client\n");
      /* Скорее всего, сегмент уже существует - попробуем как клиент */
      if((shmid = shmget(key, SEGSIZE, 0)) == -1)
      {
         perror("shmget");
         exit(1);
      }else
   {
      printf("Creating new shared memory segment\n");
   }
   }
  if((segptr = shmat(shmid, 0, 0)) == -1)
        {
         perror("shmat");
          exit(1);
        }
  Inversion(argv[0], strlen(argv[0]));
   strcpy(segptr,argv[0]);
   shmdt(segptr);
  //shmctl(shmid,IPC_RMID,0);
}
