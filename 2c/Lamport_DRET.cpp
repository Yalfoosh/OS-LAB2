/* 
 * File:   Lamport_DRET.cpp
 * Author: soul
 *
 * Lamportov algoritam dretvama.
 */

#include <cstdlib>
#include <ctime>
#include <sys/unistd.h>
#include <pthread.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <cstdio>

using namespace std;

enum radnja
{
    ulaz,
    izlaz
};

bool *Searching;
int *Order;
int *TableVector;

int tables;
int threads;

int arrMax(int arr[], int size)
{
    int max = arr[0];
    
    for(int i = 0; i < size; ++i)
        if(arr[i] > max)
            max = arr[i];
    
    return max;
}

bool isFilled(int arr[], int size)
{
    for(int i = 0; i < size; ++i)
        if(arr[i] == 0)
            return false;
    
    return true;
}

int chooseOneFree(int arr[], int size)
{
    if(isFilled(arr, size))
        return -1;
    else
    {
        std::vector<int> freeOnes;
        
        for(int i = 0; i < size; ++i)
            if(arr[i] == 0)
                freeOnes.push_back(arr[i]);
        
        return rand() % freeOnes.size();
    }
}

void printState()
{
    string toPrint;
    
    for(int i = 0; i < tables; ++i)
        toPrint.append (TableVector[i] ? "|" + to_string (TableVector[i]) + "|  ": "|-|  ");
    
    toPrint.append("\n");
    cout << toPrint;
}

void KO(int i, radnja x)
{
    if(x == ulaz)
    {
        Searching[i] = true;
        Order[i] = arrMax(Order, threads) + 1;
        Searching[i] = false;
        
        for(int j = 0; j < threads; ++j)
        {
            while(Searching[j]);
            while(Order[j] != 0 && (Order[j] < Order[i] || (Order[j] == Order[i] && j < i)));
        }
    }
    else
        Order[i] = 0;
}

void *Reserve(void* i)
{
    int I = *((int*)i);
    
    int one = chooseOneFree(TableVector, tables);
    
    if(one == -1)
    {
        string toPrint = "Nije bilo dostupnih stolova!\nDretva " + to_string(I) + " izlazi...\n";
        cout << toPrint;
        pthread_exit(i);
    }
    else
    {
        string toPrint = "Dretva " + to_string(I) + ":odabirem stol " + to_string(one + 1) + "\n";
        cout << toPrint;
    }
    
    sleep(1);
    
    KO(I, ulaz);
    
    string toPrint = "Dretva " + to_string(I) + ": ";
    
    if(TableVector[one] == 0)
    {
        TableVector[one] = I;
        
        toPrint += "rezerviram stol " + to_string(one + 1) + ", stanje:\n";
        cout << toPrint;
    }
    else
    {
        toPrint += "neuspjela rezervacija stola " + to_string(one + 1) + ", stanje:\n";
        cout << toPrint;
    }
    
    printState();
    KO(I, izlaz);
    
    pthread_exit(i);
}

int main (int argc, char** argv)
{
    srand(time(NULL));
    
    if(argc != 3)
    {
        printf("Krivi unos.\n");
        exit(1);
    }
    
    threads = atoi(argv[1]);
    tables = atoi(argv[2]);
    
    Searching = (bool*)malloc(threads * sizeof(bool));
    Order = (int*)malloc(threads * sizeof(int));
    TableVector = (int*)malloc(tables * sizeof(int));
    
    pthread_t ID[threads];
    
    for(int i = 0; i < threads; ++i)
    {
        Searching[i] = false;
        Order[i] = 0;
    }
    
    for(int i = 0; i < tables; ++i)
        TableVector[i] = 0;
    
    for(int i = 0; i < threads; ++i)
    {
        int *x = (int*)malloc(sizeof(int));
        *x = i + 1;
        
        if(pthread_create(&ID[i], NULL, Reserve, x) != 0)
        {
            printf("Neuspjelo stvaranje dretve.\n");
            exit(1);
        }
	}
    
	for(int i = 0; i < threads; ++i) 
        pthread_join(ID[i], NULL);
    
    return 0;
}
