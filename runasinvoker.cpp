///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2014 - assarbad.net
///
/// Original filename: runasinvoker.cpp
/// Project          : runasinvoker
/// Date of creation : 2014-07-02
/// Author(s)        : Oliver Schneider
///
/// Purpose          : <description>
///
/// Revisions:
///  0000 [2014-07-02] Initial revision.
///
///////////////////////////////////////////////////////////////////////////////

// $Id$

///////////////////////////////////////////////////////////////////////////////
// #define UNICODE
// #define _UNICODE
// These two defines are given implicitly through the settings of C_DEFINES in
// the SOURCES file of the project. Hence change them there and there only.
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "runasinvoker.h"

int __cdecl _tmain(int argc, _TCHAR *argv[])
{
    using namespace std;

    cout << "Hello world" << endl;
/*
    _tprintf(_T("Hello world\n"));
*/
    return 0;
}
