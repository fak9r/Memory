/*
* Memory.hpp Updated 03.09.2022
* Build: Relase x86
* Includes: Windows.h, TlHelp32.h, thread
* Defines: sleep(x) std::this_thread::sleep_for(std::chrono::milliseconds(x)), PTR uintptr_t
*/

class Memory {
private:
    static HANDLE handle;
    static PTR pid;
    static HWND hwnd;

public:
    static void setProcess(const char* szName) { // Counter-Strike: Global Offensive - Direct3D 9
        do {
            hwnd = FindWindowA(0, szName);
            sleep(50);
        } while (!hwnd);

        DWORD temp;
        GetWindowThreadProcessId(hwnd, &temp);
        pid = temp;

        handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    template <typename Type>
    static Type Read(PTR input) {
        Type temp;
        ReadProcessMemory(handle, (LPVOID)input, &temp, sizeof(temp), 0);
        return temp;
    }

    static std::string strRead(PTR input) {
        char temp[128];
        ReadProcessMemory(handle, (PBYTE*)input, &temp, sizeof(temp), 0);
        return std::string(temp);
    }

    template <typename Type>
    static void Write(PTR input, Type output) {
        WriteProcessMemory(handle, (LPVOID)input, &output, sizeof(output), 0);
    }

    static PTR getBase(const char* szModuleName) { // client.dll, engine.dll
        PTR dwModuleBaseAddress = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 ModuleEntry32;
            ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
            if (Module32First(hSnapshot, &ModuleEntry32)) {
                do {
                    if (strcmp(ModuleEntry32.szModule, szModuleName) == 0) {
                        dwModuleBaseAddress = (PTR)ModuleEntry32.modBaseAddr;
                        break;
                    }
                } while (Module32Next(hSnapshot, &ModuleEntry32));
            }
            CloseHandle(hSnapshot);
        }
        return dwModuleBaseAddress;
    }
};

HANDLE Memory::handle = NULL;
PTR Memory::pid = NULL;
HWND Memory::hwnd = NULL;

// int main() {
//     Memory::setProcess("Counter-Strike: Global Offensive - Direct3D 9");
//     PTR client = Memory::getBase("client.dll");
//
//     std::cout << Memory::Read<int>(client + 4) << std::endl;
// }