#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
using namespace std;

struct ThreadInfo
{
    string name;
    int priority;
};

DWORD WINAPI PrioWorker(LPVOID param)
{
    ThreadInfo* info = static_cast<ThreadInfo*>(param);
    HANDLE hCurrentThread = GetCurrentThread();
    BOOL success = SetThreadPriority(hCurrentThread, info->priority);

    if (success == FALSE)
    {
        cerr << "Set ThreadPriority failed, error:" << GetLastError() << endl;
    }
    
    auto start = chrono::high_resolution_clock::now();
    cout << "Started, Thread Priority is: " << info->priority << endl;

    long long counter = 0;
    for (long long i = 0; i < 10000000; i++)
    {
        counter++;
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-start;

    cout << "Finished Prio " << info->priority <<" Dauer in sec: " << diff.count() << endl;
    delete info;
    return 0;
}

int main()
{
    ThreadInfo* highPrioInfo = new ThreadInfo;
    highPrioInfo -> name="HighPrioThread";
    highPrioInfo -> priority = THREAD_PRIORITY_HIGHEST;

    HANDLE highPrioThread = CreateThread(
        NULL, // Sicherheit
        0, // Stacksize, 0=default
        PrioWorker, // Code
        highPrioInfo,
        0, // 0: sofort starten
        NULL
        );

    if (NULL == highPrioThread)
    {
        cerr << "Create Thread failed! Fehlercode: " << GetLastError() << endl;
        delete highPrioInfo;
        return 1;
    }
    
    ThreadInfo* lowPrioInfo = new ThreadInfo;
    lowPrioInfo -> name="LowPrioThread";
    lowPrioInfo -> priority = THREAD_PRIORITY_LOWEST;

    HANDLE lowPrioThread = CreateThread(
        NULL, // Sicherheit
        0, // Stacksize, 0=default
        PrioWorker, // Code
        lowPrioInfo,
        0, // 0: sofort starten
        NULL
        );

    if (NULL == lowPrioThread)
    {
        cerr << "Create Thread failed! Fehlercode: " << GetLastError() << endl;
        delete lowPrioInfo;
        return 1;
    }

    HANDLE handles[] = {highPrioThread, lowPrioThread};
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);

    CloseHandle(highPrioThread);
    CloseHandle(lowPrioThread);
    // delete highPrioInfo;
    // delete lowPrioInfo;

    return 0;
}
