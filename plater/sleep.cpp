#ifndef MSVC
#include <unistd.h>
#endif
#ifdef WIN32
#include <windows.h>
#endif

#include "sleep.h"

void ms_sleep(long ms) {
#ifdef WIN32
	Sleep(ms);
#else
    usleep(1000 * ms);
#endif
}  

void ms_sys_wait_ms(long ms)
{
#ifdef WIN32
    Sleep(ms);
#else
    usleep(1000 * ms);
#endif
}

void u_sleep(long us)
{
#ifdef WIN32
    Sleep(us/1000.0);
#else
    usleep(us);
#endif
}
