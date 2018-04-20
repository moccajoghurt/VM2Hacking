#include "Client.h"
#include "../MemWars/MemWarsCore/MemWarsCore.h"
#include <iostream>

using namespace std;

void Client::Init() {
    wstring we = L"";
    string e = "";
    lsassExe = we+L'l'+L's'+L'a'+L's'+L's'+L'.'+L'e'+L'x'+L'e';
    wVermintideExe = we+L'v'+L'e'+L'r'+L'm'+L'i'+L'n'+L't'+L'i'+L'd'+L'e'+L'2'+L'.'+L'e'+L'x'+L'e';
    vermintideExe = e+'v'+'e'+'r'+'m'+'i'+'n'+'t'+'i'+'d'+'e'+'2'+'.'+'e'+'x'+'e';
}

int main() {

    Client c;
    c.Init();

    StealthyMemClient smc;
    if (!smc.Init(c.lsassExe)) {
        cout << "Init failed" << endl;
        return 1;
    }
    if (!smc.SetTargetProcessHandle(c.wVermintideExe)) {
        cout << "Setting Handle failed" << endl;
        return 1;
    }

    uintptr_t imageBase = 140000000;
    // for (int i = 0; i < 10000; i++) {
    //     char buf = 0;
    //     SIZE_T sizeBuf;
    //     smc.ReadVirtualMemory((void*)(imageBase + i), &buf, sizeof(buf), &sizeBuf);
    //     cout << hex << (int)buf << endl;
    // }
    UINT i = 0;
    UINT val = 0;
    while (TRUE) {
        char* searchVal = "enemy_velocity";
        char* buf[255];
        SIZE_T sizeBuf;
        smc.ReadVirtualMemory((void*)(imageBase + i), &buf, strlen(searchVal), &sizeBuf);
        if (memcmp(buf, searchVal, strlen(searchVal)) == 0) {
            break;
        }
        i++;
    }
    cout << UINT(imageBase + i) << endl;

    // todo: mit "players" string als hex values suchen und kontrollieren, ob es richtig ist
    

    // HANDLE gameHandle = GetProcessByName(c.vermintideExe.c_str());
    //  HANDLE gameHandle = GetProcessByWindowName("Vermintide 2");
    // if (!gameHandle) {
    //     cout << "Retrieving Handle failed" << endl;
    //     return 1;
    // }
    // HMODULE baseAddr = (HMODULE)GetProcessBaseAddress(gameHandle);
    // cout << GetLastError() << endl;
    // cout << baseAddr << endl;
}