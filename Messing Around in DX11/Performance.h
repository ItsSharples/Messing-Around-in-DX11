#pragma once

#include "CPUUsageTimer.h"
#include <Psapi.h>


namespace Performance
{

namespace
{
bool has_started_cpu_timer = false;

PROCESS_MEMORY_COUNTERS_EX ram_struct;
}
namespace CPU
{
double GetUsage();
};

namespace RAM
{
size_t GetUsage();

void updateRamStruct();
};


};

