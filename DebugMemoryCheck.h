#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>

class DebugMemoryCheck
{
    _CrtMemState start;
public:
    DebugMemoryCheck()
    {
        // Send CRT debug reports to stdout so the console (or recording) shows them
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

        // Snapshot heap at start
        _CrtMemCheckpoint(&start);
    }

    ~DebugMemoryCheck()
    {
        _CrtMemState end, diff;
        _CrtMemCheckpoint(&end);

        if (_CrtMemDifference(&diff, &start, &end))
        {
            std::cout << "Memory differences detected:\n";
            _CrtMemDumpStatistics(&diff);
            // Dumps all objects allocated since 'start' (shows file/line if available)
            _CrtMemDumpAllObjectsSince(&start);
            // Also produce the standard leak report (if any)
            _CrtDumpMemoryLeaks();
        }
        else
        {
            std::cout << "No memory leaks detected.\n";
        }
    }

    // Optional: break on a specific allocation number to investigate a leak
    static void breakOnAlloc(long allocNumber)
    {
        _CrtSetBreakAlloc(allocNumber);
    }
};

#else

// Release builds: no-op lightweight type so you can keep the same code
class DebugMemoryCheck
{
public:
    DebugMemoryCheck() {}
    ~DebugMemoryCheck() {}
    static void breakOnAlloc(long) {}
};

#endif