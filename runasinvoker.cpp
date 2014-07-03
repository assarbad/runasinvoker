///////////////////////////////////////////////////////////////////////////////
///
/// Written 2014, Oliver Schneider (assarbad.net) - PUBLIC DOMAIN/CC0
///
/// Original filename: runasinvoker.cpp
/// Project          : runasinvoker
/// Date of creation : 2014-07-02
/// Author(s)        : Oliver Schneider
///
/// Purpose          : Start a defined program with RunAsInvoker as application
///                    compatibility option. This avoids elevation when
///                    RunAsHighest is otherwise assumed by Windows.
///
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// #define UNICODE
// #define _UNICODE
// These two defines are given implicitly through the settings of C_DEFINES in
// the SOURCES file of the project. Hence change them there and there only.
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <tchar.h>
#include "runasinvoker.h"

#ifndef ERROR_ELEVATION_REQUIRED
#define ERROR_ELEVATION_REQUIRED 740L
#endif

namespace
{
    const LPCTSTR sIEO = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options");
    const LPCTSTR sRAI = _T("RunAsInvoker_Launcher");
}

static BOOL create_process(LPCTSTR executable, LPTSTR command_line, STARTUPINFO& si, PROCESS_INFORMATION& pi)
{
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    if(::CreateProcess(executable, command_line, NULL, NULL, FALSE, DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &si, &pi))
    {
        ::DebugActiveProcessStop(pi.dwProcessId);
        ::ResumeThread(pi.hThread);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
        return TRUE;
    }
    return FALSE;
}

typedef CVerySimpleBuf<TCHAR> TSTR;

static TSTR GetModulePath()
{
    TSTR retval(32*1024);
    if(retval.getCount() > ::GetModuleFileName(NULL, retval.getBuf(), retval.getCount()))
    {
        return retval;
    }
    return (TCHAR*)0;
}

static BOOL is_registered(LPCTSTR executable)
{
    LONG regRes;
    ATL::CRegKey hkRegPath;
    TSTR regPath = sIEO;
    regPath += _T("\\");
    regPath += sRAI;
    regPath += _T("\\");
    regPath += executable;
    _tprintf(_T("Checking path '%s'\n"), regPath.getBuf());
    if(ERROR_SUCCESS == (regRes = hkRegPath.Open(HKEY_LOCAL_MACHINE, regPath.getBuf(), KEY_READ)))
    {
        hkRegPath.Close();
        _tprintf(_T("It's registered!\n"));
        return TRUE;
    }
    return FALSE;
}

static BOOL launch_process(LPCTSTR executable, int argc, _TCHAR *argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    for(int i = 0; i < argc; i++)
    {
        _tprintf(_T("Argument %i: %s\n"), i, argv[i]);
    }
    _tprintf(_T("Now trying to start: %s\n"), executable);
    if(create_process(executable, NULL, si, pi))
    {
        _tprintf(_T("PID: %u\n"), pi.dwProcessId);
        return TRUE;
    }
    else
    {
        if((ERROR_ELEVATION_REQUIRED == GetLastError()) && is_registered(executable))
        {
            ::SetEnvironmentVariable(_T("__COMPAT_LAYER"), _T("RUNASINVOKER"));
            _tprintf(_T("Now retrying to start with __COMPAT_LAYER=RUNASINVOKER: %s\n"), executable);
            if(create_process(executable, NULL, si, pi))
            {
                _tprintf(_T("PID: %u\n"), pi.dwProcessId);
                return TRUE;
            }
        }
        _tprintf(_T("Process creation failed (Win32 Error: %u).\n"), GetLastError());
    }
    return FALSE;
}

static int press_key()
{
    _tprintf(_T("\nPress any key to continue\n"));
    return getchar();
}

static int register_debugger(LPCTSTR executable)
{
    int retval = 1;
    if(::PathFileExists(executable))
    {
        LPCTSTR lastSlash = _tcsrchr(executable, _T('\\'));
        TSTR modpath = GetModulePath();
        if(lastSlash && modpath)
        {
            LONG regRes;
            LPCTSTR execfile = &lastSlash[1];
            _tprintf(_T("execfile == %s\n"), execfile);
            ATL::CRegKey hkIEO;
            if(ERROR_SUCCESS == (regRes = hkIEO.Open(HKEY_LOCAL_MACHINE, sIEO, KEY_WRITE | KEY_READ)))
            {
                ATL::CRegKey hkExecutable;
                if(ERROR_SUCCESS == (regRes = hkExecutable.Open(hkIEO, execfile, KEY_WRITE | KEY_READ)))
                {
                    if(ERROR_SUCCESS == (regRes = hkExecutable.SetValue(modpath.getBuf(), _T("Debugger"))))
                    {
                        _tprintf(_T("Successfully set the 'Debugger' to '%s' for '%s'.\n"), modpath.getBuf(), execfile);
                        retval = 0;
                    }
                    else
                    {
                        _tprintf(_T("ERROR: Failed to set value for 'Debugger' under '%s\\%s'  (%u)\n"), sIEO, execfile, regRes);
                        regRes = ERROR_SUCCESS;
                    }
                    hkExecutable.Close();
                }
                if(0 == retval)
                {
                    retval = 1;
                    ATL::CRegKey hkLauncher;
                    TSTR cfgKeyName = sRAI;
                    cfgKeyName += _T("\\");
                    cfgKeyName += executable;
                    if(ERROR_SUCCESS == (regRes = hkLauncher.Create(hkIEO, cfgKeyName.getBuf())))
                    {
                        _tprintf(_T("Successfully registered path '%s'.\n"), executable);
                        retval = 0;
                        hkLauncher.Close();
                    }
                    else
                    {
                        _tprintf(_T("ERROR: Failed to open/create registry key '%s' (%u)\n"), sIEO, regRes);
                    }
                }
                hkIEO.Close();
            }
            else
            {
                _tprintf(_T("ERROR: Failed to open registry key '%s' (%u)\n"), executable, regRes);
            }
        }
    }
    else
    {
        _tprintf(_T("ERROR: The given executable '%s' does not exist or is not a file!\n"), executable);
    }
    return retval;
}

static void show_help()
{
    _tprintf(_T("This program expects at least one parameter, which is the full path to an executable.\n\
Additionally more parameters can be passed which are to be passed to the executable upon process creation.\n\
\n\
Alternative syntax: runasinvoker --register <full path to executable>\n\
\t... when run elevated will set the 'Image Execution Options' for the given binary.\n\
"));
}

int __cdecl _tmain(int argc, _TCHAR *argv[])
{
    if(argc < 2)
    {
        show_help();
        return 1;
    }
    if((3 == argc) && (0 == lstrcmpi(_T("--register"), argv[1])) || (0 == lstrcmpi(_T("-r"), argv[1])))
    {
        LPCTSTR executable = argv[2];
        return register_debugger(executable);
    }
    return launch_process(argv[1], argc - 1, &argv[1]) ? press_key() : press_key();
}
