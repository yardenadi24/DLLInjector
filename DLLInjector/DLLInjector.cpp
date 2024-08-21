#include <iostream>
#include <Windows.h>

int main(int argc, char* argv[])
{
    int status = 0;
    if (argc != 3)
    {
        printf("Usage: DllInjector.exe <Process id> <Dll path>\n");
        return 0;
    }

    DWORD ProcessId = atoi(argv[1]);
    char* DllFullPath = argv[2];

    HANDLE hTargetProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
    if (hTargetProc == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open process (%u)\n", ProcessId);
        return -1;
    }

    // Now we have an handle to the target process and the full path of the dll
    // next steps are:
    // 1. Allocate memory for the dll path in the target process memory
    // 2. Write to the allocated memory the dll path
    // 3. Create a remote thread in the target process which will run LoadLibraryA


    // 1 - allocate
    const LPVOID lpDllPathAddress = VirtualAllocEx(hTargetProc, nullptr, strlen(DllFullPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (lpDllPathAddress == nullptr)
    {
        printf("Failed to allocate the dll path memory in the target process\n");
        CloseHandle(hTargetProc);
        return -1;
    }

    printf("Successfully allocated memory for the dll path at (0x%p)\n", lpDllPathAddress);

    // 2 - write
    if (!WriteProcessMemory(hTargetProc, lpDllPathAddress, DllFullPath, strlen(DllFullPath) + 1, 0))
    {
        printf("Failed to write the dll path memory in the target process\n");
        VirtualFreeEx(hTargetProc, lpDllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hTargetProc);
        return -1;
    }

    printf("Successfully wrote dll path to memory [PID:%u , Path:%s]\n",ProcessId , DllFullPath);

    // 3 - create remote thread that loads the DLL
    const HANDLE hRemoteThreadCreationRes = CreateRemoteThread(hTargetProc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, lpDllPathAddress, 0, NULL);
    if (hRemoteThreadCreationRes == INVALID_HANDLE_VALUE)
    {
        VirtualFreeEx(hTargetProc, lpDllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hTargetProc);
        printf("Failed creating remote thread\n");
        return -1;
    }


    printf("DLL: (%s) injected to process with ID: (%u)\n", DllFullPath, ProcessId);
    return 0;
}
