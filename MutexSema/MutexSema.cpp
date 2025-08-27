#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
using namespace std;

int sharedCounter = 0;

HANDLE hMutex;
HANDLE hSemaphore;

DWORD WINAPI MutexWorker(LPVOID lpParam)
{
    string threadName((const char*) lpParam);
    cout << threadName << " beantragt Mutex " << endl;
    DWORD waitResult = WaitForSingleObject(hMutex, INFINITE);
    switch (waitResult)
    {
    case WAIT_OBJECT_0:
        cout << threadName << " hat Mutex erhalten " << endl;
        for (int i = 0; i < 5;i++)
        {
            sharedCounter++;
        }
        Sleep(100);
        break;
        
    case WAIT_ABANDONED:
        break;
    }
    
    cout << threadName << " gibt Mutex frei" << endl;
    
    if (!ReleaseMutex(hMutex))
    {
        cout << threadName << " Mutex nicht freigebbar" << endl;
    }
    return 1;
}

int main()
{
    sharedCounter = 0;
    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL)
    {
        cerr << "CreateMutex failed " << GetLastError() << endl;
        return 1;
    }

    HANDLE hMutexThreads[5];
    const char* threadNames[5] = {"Thread1Mutex","Thread2Mutex","Thread3Mutex","Thread4Mutex","Thread5Mutex"};
    for (int i = 0; i<5;i++)
    {
        hMutexThreads[i] = CreateThread(
            NULL,
            0,
            MutexWorker,
            (LPVOID)threadNames[i],
            0,
            NULL
            );
        if (hMutexThreads[i] == NULL)
        {
            cerr << "CreateMutex failed " << GetLastError() << endl;
            return 1;
        }
    }
    
    WaitForMultipleObjects(5, hMutexThreads, TRUE, INFINITE);
    
    for (int i = 0; i<5; i++)
    {
        CloseHandle(hMutexThreads[i]);
    }
    CloseHandle(hMutex);
    return 0;
}
