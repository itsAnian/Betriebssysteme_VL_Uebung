#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

void startNewProcess()
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    LPCSTR applicationName = "c:/windows/notepad.exe";
    cout << "Start a new notepad process" << endl;

    BOOL success = CreateProcessA(
        applicationName, // Programm
        NULL, // Befehlszeile
        NULL, // Sicherheit Prozess
        NULL, // Sicherheit des Thread
        FALSE, // Handles
        0, // Erstellungsflags
        NULL, // Enviroment
        NULL, // dir
        &si,
        &pi
        );
    if (success)
    {
        cout << "Process successfully created" << endl;
        cout << "Process ID:" << pi.dwProcessId << endl;
        cout << "Thread ID:" << pi.dwThreadId << endl;

        WaitForSingleObject(pi.hProcess, INFINITE);
        cout << "Process terminated" << endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        cerr << "Process could not be created" << GetLastError() << endl;
    }
}

int main()
{
    startNewProcess();
    return 0;
}
