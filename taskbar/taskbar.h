// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the TASKBAR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// TASKBAR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TASKBAR_EXPORTS
#define TASKBAR_API __declspec(dllexport)
#else
#define TASKBAR_API __declspec(dllimport)
#endif

extern "C"
{
    TASKBAR_API BOOL WINAPI InstallHook(void);
    TASKBAR_API void WINAPI UninstallHook(void);
    TASKBAR_API void WINAPI SetTaskbarMode(int mode, int transopca);
    TASKBAR_API void WINAPI SetOptions(BOOL bMaximumWndMatch);
}
