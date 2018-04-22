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
    // wVermintideExe = we+L'v'+L'e'+L'r'+L'm'+L'i'+L'n'+L't'+L'i'+L'd'+L'e'+L'2'+L'.'+L'e'+L'x'+L'e';
    wVermintideExe = L"RustClient.exe";
    vermintideExe = e+'v'+'e'+'r'+'m'+'i'+'n'+'t'+'i'+'d'+'e'+'2'+'.'+'e'+'x'+'e';
}

BOOL Client::FindValue(void* value, const SIZE_T size, MEMPTRS* matchingValues, uintptr_t startAddress = baseAddress, uintptr_t endAddress = endOfGameAddress) {
    if (size > MAX_VAL_SIZE) {
        return FALSE;
    }
    for (uintptr_t i = startAddress; i < endAddress; i++) {
        SIZE_T sizeBuf;
        UCHAR buf[MAX_VAL_SIZE];
        smc.ReadVirtualMemory((void*)(i), &buf, size, &sizeBuf);
        if (memcmp(value, buf, size) == 0) {
            ConcatMemPtr((void*)i, matchingValues);
        }
    }
    return TRUE;
}

BOOL Client::FindIntegerRoutine() {
    MEMPTRS ptrs = {0};
    int val;
    cout << "Enter Integer: ";
    cin >> val;
    if (!FindValue(&val, sizeof(int), &ptrs)) {
        return FALSE;
    }
    while (TRUE) {
        cout << "show: -1, any other int: filter" << endl;
        cin >> val;
        if (val != -1) {
            MEMPTRS newPtrs = {0};
            for (UINT i = 0; i < ptrs.size; i++) {
                int buf;
                SIZE_T sizeBuf;
                smc.ReadVirtualMemory((void*)ptrs.memPointerArray[i], &buf, sizeof(int), &sizeBuf);
                if (buf == val) {
                    ConcatMemPtr((void*)ptrs.memPointerArray[i], &newPtrs);
                }
            }
            ptrs = newPtrs;
        } else {
            for (UINT i = 0; i < ptrs.size; i++) {
                cout << hex << ptrs.memPointerArray[i] << endl;
            }
        }
    }
    return TRUE;
}

map<uintptr_t, int> Client::GetMemoryMap(uintptr_t startAddress = baseAddress, uintptr_t endAddress = endOfGameAddress) {
    map<uintptr_t, int> memoryMap;
    for (uintptr_t i = startAddress; i < endAddress; i++) {
        SIZE_T sizeBuf;
        int buf;
        smc.ReadVirtualMemory((void*)(i), &buf, sizeof(int), &sizeBuf);
        memoryMap[i] = buf;
    }
    return memoryMap;
}

BOOL Client::MemoryMapRoutine(uintptr_t startAddress = baseAddress, uintptr_t endAddress = endOfGameAddress) {
    cout << "Creating Map..." << endl;
    map<uintptr_t, int> memoryMap = GetMemoryMap(startAddress, endAddress);

    while (TRUE) {
        int choice;
        cout << "1: keep vals that changed" << endl << "2: keep vals that didn't change" << endl
        << "3: keep vals that got smaller" << endl << "4: keep vals that got bigger" << endl;
        cin >> choice;
        if (choice < 1 || choice > 4) {
            cout << "invalid choice" << endl;
            return FALSE;
        }
        for(map<uintptr_t, int>::iterator i = memoryMap.begin(); i != memoryMap.end();) {
            SIZE_T sizeBuf;
            int buf;
            smc.ReadVirtualMemory((void*)(i->first), &buf, sizeof(int), &sizeBuf);

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
                for(map<uintptr_t, int>::iterator i = memoryMap.begin(); i != memoryMap.end();) {
                    cout << hex << i->first << "\t" << dec << i->second << endl;
                    i++;
                }
            }
        }
    }



    return TRUE;
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

    c.MemoryMapRoutine();
}