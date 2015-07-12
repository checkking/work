#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int myid; // the rank id of this process
    int np;   // the number of processes
    int  N; // dimension of the vector
    int beishu;
    int yushu;
    int start_idx; // the start compute index
    int end_idx;  // the end compute index
    int* vector;   // the vector
    unsigned long long result = 0;    // the final result
    int receive_cnt = np -1;
    int* buff; // receive buffer
    int len; // buffer data length
    int rand_num; // random number.
        
    int i;
    MPI_Status status;

    if(argc != 2 ) return 1;    
    N = atoi(argv[1]);
    if(N <= 0) return 1;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid); 
    MPI_Comm_size(MPI_COMM_WORLD,&np);
printf("myid=%d\n",myid);
    vector = (int *)malloc(sizeof(int)*N);
    if(vector == NULL)
    {
        printf("malloc failed!\n");
        MPI_Finalize();
        return 1;
    }
    buff = (int *)malloc(sizeof(int)*N);
    if(buff == NULL)
    {
        printf("malloc failed!\n");
        MPI_Finalize();
        return 1;
    }
    for(i=0; i < N; i++)
        vector[i] = i+1;
    beishu  = N/np;
    yushu  = N%np;
printf("%d...%d\n",beishu,yushu);
    start_idx = myid*beishu;
    if(myid < yushu)
        start_idx += myid;
    else 
        start_idx += yushu;
    end_idx = start_idx + beishu - 1;
    if(myid < yushu)
        end_idx += 1;
printf("start idx = %d, end_index= %d\n",start_idx, end_idx);
    //1. compute part of the vector in this rank id
    for( i = start_idx; i <= end_idx; i++)
        vector[i] = vector[i]*vector[i];
for(i = 0; i < N; i++)
    printf("%d,", vector[i]);
printf("\n");

    //2. send the part of result to other rank ids
    if(end_idx >= start_idx)
        for(i = 0; i < np; i++)
            if(i != myid)
                MPI_Send((void *)(vector+start_idx), end_idx-start_idx+1,MPI_INT,i,99,MPI_COMM_WORLD);
    
    //3. receive the other part results from other rank ids
    // compute the receive count of this process
    if(beishu == 0) //N < np
        if(myid < yushu)
            receive_cnt = yushu -1;
        else 
            receive_cnt = yushu;
    else 
        receive_cnt = np - 1;
    printf("receive cnt:%d\n", receive_cnt);
    while(receive_cnt) {
        memset(buff, 0, N*sizeof(int));
        MPI_Recv(buff, N, MPI_INT,MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        len = 0;
        MPI_Get_count(&status, MPI_INT, &len);
        start_idx = status.MPI_SOURCE*beishu;
        if(status.MPI_SOURCE < yushu)        
            start_idx += status.MPI_SOURCE;
        else 
            start_idx += yushu;
        for(i = 0; i < len; i++)
            vector[start_idx+i] = buff[i];
        receive_cnt--;
        printf("receive cnt:%d\n", receive_cnt);
    }
for(i = 0; i < N; i++)
    printf("%d,", vector[i]);
    printf("\n");
    //4. transformation 
    srand((unsigned)time( NULL )+myid);
    rand_num = rand() % 100 +1;
    printf("random number = %d\n", rand_num);
    for(i = 0; i < N; i++)
        result += vector[i] * rand_num;                    
    printf("%llu\n", result);

    free(vector);
    free(buff);
    
    MPI_Finalize();

    return 0;
}
