// Based on https://stackoverflow.com/a/46009187


#include "pch.h"
#include "CPUUsageTimer.h"



namespace CPU_Usage
{
__int64 FileTimeToInt64(FILETIME& ft)
{
    ULARGE_INTEGER foo;

    foo.LowPart = ft.dwLowDateTime;
    foo.HighPart = ft.dwHighDateTime;

    return (foo.QuadPart);
}


// UI Timer callback

void CALLBACK UITimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
#define NUMBER_OF_PROCESSORS (12)
#define PROCESSOR_BUFFER_SIZE (NUMBER_OF_PROCESSORS * 8)

    static ULONG64 ProcessorIdleTimeBuffer[PROCESSOR_BUFFER_SIZE];
    std::string  ProcessorHeadroomPercentage = "Headroom: %2.0lf%%   Load: %2.0lf%%\n";

    FILETIME IdleTime, KernelTime, UserTime;
    FILETIME CreationTime, ExitTime, KernelProcess, UserProcess;

#define time_t  unsigned long long
    static time_t PrevTotal = 0;
    static time_t PrevIdle = 0;
    //static time_t PrevUser = 0;

    static time_t PrevProcessTotal = 0;
    //static time_t PrevProcessIdle = 0;
    //static time_t PrevProcessUser = 0;

    time_t ThisTotal;
    time_t ThisIdle, ThisKernel, ThisUser;

    time_t ProcessTotal;
    time_t ProcessCreation, ProcessKernel, ProcessUser;

    time_t TotalSinceLast, IdleSinceLast, UserSinceLast;

    time_t ProcessTotalSinceLast, ProcessKernelSinceLast, ProcessUserSinceLast;
    

    // GET THE KERNEL / USER / IDLE times.  
    // And oh, BTW, kernel time includes idle time
    GetSystemTimes(&IdleTime, &KernelTime, &UserTime);
    GetProcessTimes(GetCurrentProcess(), &CreationTime, &ExitTime, &KernelProcess, &UserProcess);

    ThisIdle = FileTimeToInt64(IdleTime);
    ThisKernel = FileTimeToInt64(KernelTime);
    ThisUser = FileTimeToInt64(UserTime);

    ProcessCreation = FileTimeToInt64(CreationTime);
    ProcessKernel = FileTimeToInt64(KernelProcess);
    ProcessUser = FileTimeToInt64(UserProcess);

    ThisTotal = ThisKernel + ThisUser; 
    TotalSinceLast = ThisTotal - PrevTotal;
    IdleSinceLast = ThisIdle - PrevIdle;
    //UserSinceLast = ThisUser - PrevUser;

    ProcessTotal = ProcessKernel + ProcessUser;
    ProcessTotalSinceLast = ProcessTotal - PrevProcessTotal;

    double ProcessShare;
    ProcessShare = (double)ProcessTotalSinceLast / (double)TotalSinceLast;
    Headroom = (double)IdleSinceLast / (double)TotalSinceLast;

    ProcessLoad = ProcessShare * Headroom;
    ProcessHeadroom = 1.0 - ProcessLoad;
    Load = 1.0 - Headroom;

    double tmp;

    tmp = Headroom;
    tmp *= 100.0;  // to make it percent
    Headroom = tmp;

    tmp = Load;
    tmp *= 100.0;  // percent
    Load = tmp;

    PrevTotal = ThisTotal;
    PrevIdle = ThisIdle;
    //PrevUser = ThisUser;

    PrevProcessTotal = ProcessTotal;

    // print results to output window of VS when run in Debug
    //sprintf(&ProcessorHeadroomPercentage.front(), "Headroom: %2.0lf%%   Load: %2.0lf%%\n", Headroom, Load);
    //OutputDebugStringA(ProcessorHeadroomPercentage.c_str());
    //return ProcessorHeadroomPercentage;
}



void SetupMyTimer(void)
{
    // set up a timer to periodically update UI, specifically to support headroom display
    // I'll use a timerQueue for this application
    // Create the timer queue.
    HANDLE   hTimerQueue;
    HANDLE   hUITimer;
    enum { UI_TIMER_RATE = 1000 };  // should happen every 1000 ms or 1Hz.  That should be fast enough

    hTimerQueue = NULL;
    hUITimer = NULL;
    hTimerQueue = CreateTimerQueue();
    CreateTimerQueueTimer(&hUITimer, hTimerQueue,
        (WAITORTIMERCALLBACK)UITimerRoutine, NULL, 0, UI_TIMER_RATE, 0); //the 1000 means wait 1000ms for first call

}
double getLoad()
{
    return ProcessLoad;
}

}