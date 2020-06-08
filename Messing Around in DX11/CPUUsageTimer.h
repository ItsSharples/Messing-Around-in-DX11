#pragma once

#include <atomic>

namespace
{
    std::atomic<double> Headroom;
    std::atomic<double> Load;
}

namespace CPU_Usage
{
__int64 FileTimeToInt64(FILETIME& ft);
void CALLBACK UITimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void SetupMyTimer(void);
double getLoad();
}