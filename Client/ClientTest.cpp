#include "Client.h"
#include "../MemWars/MemWarsCore/MemWarsCore.h"
#include "../MemWars/MemWarsServices/MemWarsServices.h"
#include <iostream>

using namespace std;

void FindValueTest() {
    HANDLE process = NULL;
    while (process == NULL) {
        process = (HANDLE)GetProcessByName("SkypeApp.exe");
        if (process == NULL) {
            cout << "Open Skype to start testing..." << endl;
            Sleep(5000);
        }
    }

    Client c;
    c.Init();
    
    if (!c.GetMemManipClient().Init(c.GetLsassExe())) {
        cout << "FindValueTest failed: Init failed" << endl;
        return;
    }
    if (!c.GetMemManipClient().SetTargetProcessHandle(L"SkypeApp.exe")) {
        cout << "FindValueTest failed: Setting Handle failed" << endl;
        return;
    }

    HANDLE gameHandle = GetProcessHandleByName(L"SkypeApp.exe");
    if (!gameHandle) {
        cout << "FindValueTest failed: invalid handle" << endl;
        return;
    }

    BYTEARRAY bArr;
    IntToByteArray(&bArr, 1337);
    MEMPTRS testPtrs = {0};
    FindValueInProcess(&bArr, process, &testPtrs);

    vector<uintptr_t> testPtrs1 = c.FindValue(bArr.values, bArr.size, process);

    if (testPtrs1.size() == testPtrs.size) {
        cout << "FindValueTest success" << endl;
    } else {
        
        cout << "FindValueTest failed" << endl;
    }
    cout << testPtrs1.size() << endl << testPtrs.size << endl;
    // c.FindValueRoutine(gameHandle);
}

int main () {
    FindValueTest();
}