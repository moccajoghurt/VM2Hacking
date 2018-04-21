#pragma once;
#include <string>
#include <map>
#include "../MemWars/MemWarsCore/MemWarsCore.h"
#include "../MemWars/MemWarsServices/StealthyMemManipulatorClient.h"
#include "../MemWars/MemWarsServices/StealthyMemManipulatorGetHandleId.h"

#define MAX_VAL_SIZE 255

class Client {
public:
    void Init();
    BOOL FindValue(void* value, SIZE_T size, MEMPTRS* matchinValues, uintptr_t startAddress, uintptr_t endAddress);
    BOOL FindIntegerRoutine();
    map<uintptr_t, int> GetMemoryMap(uintptr_t startAddress, uintptr_t endAddress);
    BOOL MemoryMapRoutine(uintptr_t startAddress, uintptr_t endAddress);
    StealthyMemClient& GetMemManipClient() {
        return smc;
    }
    wstring GetLsassExe() {
        return lsassExe;
    }
    wstring GetWVermintideExe() {
        return wVermintideExe;
    }
    string GetVermintideExe() {
        return vermintideExe;
    }
protected:
    wstring lsassExe;
    wstring wVermintideExe;
    string vermintideExe;
    static const uintptr_t textBase = 0x10000;
    static const uintptr_t imageBase = 0x140000000;
    static const uintptr_t baseAddress = textBase + imageBase;
    static const uintptr_t endOfGameAddress = 0x14102D029;
    StealthyMemClient smc;
};

/*
Strings of interest:
lua_*
get_*
get_unit_data
mTarget*
unit_position
unit_indices
unit_rotation
local_rotation
local_position
set_local_position
world_pose
world_rotation
world_position
set_local_pose
update_position_lookup
*/