#include "Client.h"
#include "../MemWars/MemWarsCore/MemWarsCore.h"
#include "../MemWars/MemWarsServices/MemWarsServices.h"
#include <iostream>

using namespace std;

void Client::Init() {
    wstring we = L"";
    string e = "";
    lsassExe = we+L'l'+L's'+L'a'+L's'+L's'+L'.'+L'e'+L'x'+L'e';
    wVermintideExe = we+L'v'+L'e'+L'r'+L'm'+L'i'+L'n'+L't'+L'i'+L'd'+L'e'+L'2'+L'.'+L'e'+L'x'+L'e';
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

    c.FindIntegerRoutine();
    
}