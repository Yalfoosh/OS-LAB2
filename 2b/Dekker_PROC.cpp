/* 
 * File:   DekkerPROC.cpp
 * Author: soul
 *
 * Dekkerov algoritam procesima
 */

#include <cstdlib>
#include <cstdio>
#include <sys/unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

enum radnja
{
    ulaz,
    izlaz
};

struct Container
{
    int permission;
    bool flag[2];
};

int ID;
Container* PODACI;
int REPEAT = 5;
 
void KO(int i, int j, radnja x)
{
    if(x == ulaz)
    {
        (PODACI -> flag[i]) = true;
        
        while((PODACI -> flag[j]) == true)
        {
            if((PODACI -> permission) == j)
            {
                (PODACI -> flag[i]) = false;
                while((PODACI -> permission) == j);
            }
            
            (PODACI -> flag[i]) = true;
        }
    }
    else
    {
        (PODACI -> permission) = j;
        (PODACI -> flag[i]) = false;
    }
}

void ispis(int i, int k, int m)
{
    printf("Proces: %d, K.O. br: %d (%d / %d)\n", i, k, m, REPEAT);
}

void proces(int i)
{
    for(int k = 1; k <= REPEAT; ++k)
    {
        KO(i, 1- i, ulaz);
        
        for(int m = 1; m <= REPEAT; ++m, sleep(1))
            ispis(i, k, m);
        
        KO(i, 1 - i, izlaz);
    }
    
    exit(0);
}

int main (int argc, char** argv)
{
    ID = shmget(IPC_PRIVATE, sizeof(Container), 0600);
    
    if(ID == -1)
        exit(1);
    
    PODACI = (Container*)shmat(ID, NULL, 0);
    
    PODACI -> permission = -1;
    PODACI -> flag[0] = false;
    PODACI -> flag[1] = false;
    
    int i = 0, j;
    for(i = 0; i < 2; ++i)
    {
        j = fork();
        if(j == 0)
            proces(i);
        else if(j != -1)
            wait(nullptr);
    }
    
    shmdt(PODACI);
    shmctl(ID, IPC_RMID, NULL);
    
    getchar();
    
    return 0;
}