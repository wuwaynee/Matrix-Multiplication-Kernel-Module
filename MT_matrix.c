#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>

#define TEST_LIGHT "\033[1;33m"
#define NONE "\033[0m"

int **result;
int **matrix1;
int **matrix2;
pthread_mutex_t mutex;

void *threadRun(void *arg){
    int* argi = (int*)arg;  
    for(int k=argi[2];k<argi[3];k++){
        for(int i=0;i<argi[0];i++){
            int total=0;
            for(int j=0;j<argi[1];j++){
                total+=matrix1[i][j]*matrix2[j][k];
            }
            result[i][k]=total;
        }
    }
    pid_t tid=gettid();
    char data[BUFSIZ];
    sprintf(data,"%d",tid);
    int fd = open("/proc/thread_info", O_RDWR);
    if(fd == -1)
    {
      printf("/proc/thread_info does not exist\n");
      return (void*)1;
    }
    pthread_mutex_lock(&mutex);
    write(fd, data, strlen(data));
    close(fd);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int row1;
    int column1;

    int row2;
    int column2;
    
    int threadnum = strtol(argv[1],NULL,10);
    
    FILE* input_file1=fopen(argv[2],"r");
    FILE* input_file2=fopen(argv[3],"r");
    if(!input_file1 || !input_file2) 
        exit(EXIT_FAILURE);

    fscanf(input_file1, "%d %d\n", &row1, &column1);
    fscanf(input_file2, "%d %d\n", &row2, &column2);
   
    matrix1=(int**)malloc(sizeof(int*)*row1);
    for(int i=0;i<row1;i++){
        matrix1[i]=(int*)malloc(sizeof(int)*column1);
    }

    matrix2=(int**)malloc(sizeof(int*)*row2);
    for(int i=0;i<row2;i++){
        matrix2[i]=(int*)malloc(sizeof(int)*column2);
    }

    for(int i=0;i<row1;i++){
        for(int j=0;j<column1;j++){
            fscanf(input_file1, "%d " ,&matrix1[i][j]);
        }
    }

    for(int i=0;i<row2;i++){
        for(int j=0;j<column2;j++){
            fscanf(input_file2, "%d " ,&matrix2[i][j]);
        }
    }

    result=(int**)malloc(sizeof(int*)*row1);
    for(int i=0;i<row1;i++){
        result[i]=(int*)malloc(sizeof(int)*column2);
    }

    /* dividing work */
    pthread_t t[threadnum];
    int **childthreadwork=(int**)malloc(sizeof(int*)*threadnum);
    struct timeval start;
	struct timeval end;
    gettimeofday(&start,NULL);

    for(int a=0;a<threadnum;a++){
        childthreadwork[a]=(int*)malloc(sizeof(int)*4);
        childthreadwork[a][0]=row1;
        childthreadwork[a][1]=column1;
        childthreadwork[a][2]=(column2*(a))/(threadnum);
        childthreadwork[a][3]=((column2*(a+1))/(threadnum));
        pthread_create(&t[a], NULL, threadRun, childthreadwork[a]);
    }

    for(int b=0;b<threadnum;b++){
        pthread_join(t[b], NULL);
    }
    gettimeofday(&end,NULL);


    int fd = open("/proc/thread_info", O_RDWR);
    if(fd == -1)
    {
      printf("/proc/thread_info does not exist\n");
      return 1;
    }
    char readbuffer[BUFSIZ]={"\0"};
    read(fd, readbuffer, BUFSIZ);
    printf("%s",readbuffer);
    printf("Elapsed time: %ld\n",end.tv_sec-start.tv_sec);
    
    free(result);
    free(matrix1);
    free(matrix2);
    fclose(input_file1);
    fclose(input_file2);
    close(fd);
    return 0;
}