/* 
 * File:   N_thr_Mx++.cpp
 * Author: soul
 */

#include <cstdlib>
#include <cstdio>
#include <pthread.h>

int A;

using namespace std;

void *threadLaunch(void *a)
{
    for(int m = *((int*)a), i = 0; i < m; ++i, ++A);
    
    pthread_exit(a);
}

int main(int argc,char *argv[]) 
{
    if(argc != 3)
    {
        printf("Krivi unos.\n");
        exit(1);
    }
        
    A = 0;
	int N = atoi(argv[1]);
	int M = atoi(argv[2]);
	pthread_t ID[N];

	for(int i = 0; i < N; ++i) 
    {
        if(pthread_create(&ID[i], NULL, threadLaunch, &M) != 0)
        {
            printf("Neuspjelo stvaranje dretve.\n");
            exit(1);
        }
	}
        
	for(int i = 0; i < N ; ++i) 
        pthread_join(ID[i], NULL);
        
	printf("A = %d\n", A);
    
    return 0;
}