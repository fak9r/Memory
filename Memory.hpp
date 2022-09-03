/*
* Memory.hpp Updated 03.09.2022
* Build: Relase x86
* Includes: Windows.h, TlHelp32.h, thread
* Defines: sleep(x) std::this_thread::sleep_for(std::chrono::milliseconds(x)), PTR uintptr_t
*/

class Memory {
private:
    HANDLE handle;
    PTR pid;
    HWND hwnd;

public:
    void setProcess(const char* szName) { // Counter-Strike: Global Offensive - Direct3D 9
        do {
            this->hwnd = FindWindowA(0, szName);
            sleep(50);
        } while (!this->hwnd);

        DWORD temp;
        GetWindowThreadProcessId(this->hwnd, &temp);
        this->pid = temp;

        this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pid);
    }

    template <typename Type>
    Type Read(PTR input) {
        Type temp;
        ReadProcessMemory(this->handle, (LPVOID)input, &temp, sizeof(temp), 0);
        return temp;
    }

    std::string strRead(PTR input) {
        char temp[128];
        ReadProcessMemory(this->handle, (PBYTE*)input, &temp, sizeof(temp), 0);
        return std::string(temp);
    }

    template <typename Type>
    void Write(PTR input, Type output) {
        WriteProcessMemory(this->handle, (LPVOID)input, &output, sizeof(output), 0);
    }

    PTR getBase(const char* szModuleName) { // client.dll, engine.dll
        PTR dwModuleBaseAddress = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->pid);
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
