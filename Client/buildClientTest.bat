@ECHO OFF
cl.exe /EHsc ClientTest.cpp Client.cpp ../MemWars/MemWarsServices/StealthyMemManipulatorClient.cpp ../MemWars/MemWarsServices/StealthyMemManipulatorGetHandleId.cpp ../MemWars/MemWarsServices/MemWarsServices.cpp ../MemWars/MemWarsCore/MemWarsCore.c ../MemWars/MemWarsServices/spinlock.obj /link User32.lib Kernel32.lib Advapi32.lib Ntdll.lib