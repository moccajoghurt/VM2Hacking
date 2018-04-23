#include "Client.h"
#include "../MemWars/MemWarsCore/MemWarsCore.h"
#include "../MemWars/MemWarsServices/MemWarsServices.h"
#include <iostream>
#include <map>

using namespace std;

void Client::Init() {
    wstring we = L"";
    string e = "";
    lsassExe = we+L'l'+L's'+L'a'+L's'+L's'+L'.'+L'e'+L'x'+L'e';
    wVermintideExe = we+L'v'+L'e'+L'r'+L'm'+L'i'+L'n'+L't'+L'i'+L'd'+L'e'+L'2'+L'.'+L'e'+L'x'+L'e';
    // wVermintideExe = L"RustClient.exe";
    vermintideExe = e+'v'+'e'+'r'+'m'+'i'+'n'+'t'+'i'+'d'+'e'+'2'+'.'+'e'+'x'+'e';
}

vector<uintptr_t> Client::FindValue(void* value, const SIZE_T size, HANDLE hProcess, uintptr_t startAddress = baseAddress, uintptr_t endAddress = endOfGameAddress) {
    vector<uintptr_t> ptrs;
    if (size > MAX_VAL_SIZE) {
        cout << "Val too big" << endl;
        return ptrs;
    }
    MEMORY_BASIC_INFORMATION info;
    for (PBYTE p = NULL; VirtualQueryEx(hProcess, p, &info, sizeof(info)) != 0; p += info.RegionSize) {
        if (info.State == MEM_COMMIT) {
            BYTE* buf = (BYTE*)malloc(info.RegionSize);
            SIZE_T sizeBuf;
            smc.ReadVirtualMemory(p, &buf, info.RegionSize, &sizeBuf);
            for (int i = 0; i < info.RegionSize; i++) {
                if (i + size > info.RegionSize) {
                    // end of memory region reached
                    break;
                }
                if (memcmp(buf + i, value, size) == 0) {
                    ptrs.push_back((uintptr_t)(p + i));
                }
            }
            free(buf);
        }
    }
    return ptrs;
}

BOOL Client::FindValueRoutine() {
    
}

map<uintptr_t, BYTE> Client::GetMemoryMap(uintptr_t startAddress = baseAddress, uintptr_t endAddress = endOfGameAddress) {
    map<uintptr_t, BYTE> memoryMap;
    for (uintptr_t i = startAddress; i < endAddress; i++) {
        SIZE_T sizeBuf;
        int buf;
        smc.ReadVirtualMemory((void*)(i), &buf, sizeof(BYTE), &sizeBuf);
        memoryMap[i] = buf;
    }
    return memoryMap;
}

BOOL Client::MemoryMapRoutine(uintptr_t startAddress = baseAddress, uintptr_t endAddress = endOfGameAddress) {
    cout << "Creating Map..." << endl;
    map<uintptr_t, BYTE> memoryMap = GetMemoryMap(startAddress, endAddress);

    while (TRUE) {
        int choice;
        cout << "1: keep vals that changed" << endl << "2: keep vals that didn't change" << endl
        << "3: keep vals that got smaller" << endl << "4: keep vals that got bigger" << endl;
        cin >> choice;
        if (choice < 1 || choice > 4) {
            cout << "invalid choice" << endl;
            return FALSE;
        }
        for(map<uintptr_t, BYTE>::iterator i = memoryMap.begin(); i != memoryMap.end();) {
            SIZE_T sizeBuf;
            int buf;
            smc.ReadVirtualMemory((void*)(i->first), &buf, sizeof(BYTE), &sizeBuf);

            if (choice == 1 && i->second == buf) {
                memoryMap.erase(i++);
            } else if (choice == 2 && i->second != buf) {
                memoryMap.erase(i++);
            } else if (choice == 3 && i->second <= buf) {
                memoryMap.erase(i++);
            } else if (choice == 4 && i->second >= buf) {
                memoryMap.erase(i++);
            }  else {
                ++i;
            }
        }
        choice = 1;
        while (choice == 1) {
            cout << "size: " << memoryMap.size() << endl << "1: show" << endl << "other val: no" << endl;
            cin >> choice;
            if (choice == 1) {
                for(map<uintptr_t, BYTE>::iterator i = memoryMap.begin(); i != memoryMap.end();) {
                    cout << hex << i->first << "\t" << dec << i->second << endl;
                    i++;
                }
            }
        }
    }

    return TRUE;
}

HANDLE GetProcessHandleByName(wstring name, DWORD access = PROCESS_ALL_ACCESS, BOOL inheritHandle = FALSE) {
    DWORD processID = GetPIDsOfProcess(name)[0];
    HANDLE hProc = OpenProcess(access, inheritHandle, processID);
    return hProc;
}

int main() {

    Client c;
    c.Init();
    
    if (!c.GetMemManipClient().Init(c.GetLsassExe())) {
        cout << "Init failed" << endl;
        return 1;
    }
    if (!c.GetMemManipClient().SetTargetProcessHandle(c.GetWVermintideExe())) {
        cout << "Setting Handle failed" << endl;
        return 1;
    }

    // c.MemoryMapRoutine();
    // c.FindValueRoutine();

    HANDLE gameHandle = GetProcessHandleByName(c.GetWVermintideExe());
    if (!gameHandle) {
        cout << "invalid handle" << endl;
        return 1;
    }

    MEMORY_BASIC_INFORMATION buf;
    SIZE_T result = VirtualQueryEx(gameHandle, (void*)0x140000000, &buf, sizeof(buf));
    if (!result) {
        cout << "VirtualQuery failed " << GetLastError() <<endl;
    }
    cout << result << endl;
}