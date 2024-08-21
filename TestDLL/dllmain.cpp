// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!AttachConsole(ATTACH_PARENT_PROCESS))
        {
            AllocConsole();
        }

        FILE* pFile;
        if (freopen_s(&pFile, "CONOUT$", "w", stdout)) {
            MessageBoxA(NULL, "Failed to redirect stdout.", "Error", MB_ICONERROR);
        }
        printf("DLL has been successfully injected!\n");
        printf("This message is coming from the injected DLL.\n");
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        FreeConsole();
    }
    return TRUE;
}

