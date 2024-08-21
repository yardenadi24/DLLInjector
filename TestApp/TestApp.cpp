#include <iostream>
#include <Windows.h>

int main() {
    DWORD ProcessId = GetCurrentProcessId();
    while (true) {
        std::cout << "I am waiting for injection (PID:"<< ProcessId <<" )" << std::endl;
        Sleep(5000);
    }
    return 0;
}