#include "pch.h"
#include "BufferedStdLogger.h"

bool Logging::BufferedStdLogger::flush()
{
    std::cout << buffer.str();
    BufferedLogger::flush();
    return true;
}
