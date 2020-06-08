
#include "pch.h"
#include "Performance.h"
#include "CPUUsageTimer.h"

size_t Performance::RAM::GetUsage()
{
    updateRamStruct();
    return ram_struct.WorkingSetSize;
}

void Performance::RAM::updateRamStruct()
{
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&ram_struct, sizeof(ram_struct));
}

double Performance::CPU::GetUsage()
{
    if(!has_started_cpu_timer)
    {
        CPU_Usage::SetupMyTimer();
        has_started_cpu_timer = true;
    }
    return CPU_Usage::getLoad();
}
