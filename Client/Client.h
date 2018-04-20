#pragma once;
#include <string>
#include "../MemWars/MemWarsServices/StealthyMemManipulatorClient.h"
#include "../MemWars/MemWarsServices/StealthyMemManipulatorGetHandleId.h"

class Client {
public:
    void Init();
    wstring lsassExe;
    wstring wVermintideExe;
    string vermintideExe;
};