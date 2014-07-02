///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2014 - assarbad.net
///
/// Defines for the version information in the resource file
///
/// (File was in the PUBLIC DOMAIN  - Created by: ddkwizard\.assarbad\.net)
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifndef __EXEVERSION_H_VERSION__
#define __EXEVERSION_H_VERSION__ 100

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "buildnumber.h"

// ---------------------------------------------------------------------------
// Several defines have to be given before including this file. These are:
// ---------------------------------------------------------------------------
#define TEXT_AUTHOR            Oliver Schneider // author (optional value)
#define PRD_MAJVER             1 // major product version
#define PRD_MINVER             0 // minor product version
#define PRD_BUILD              0 // build number for product
#define FILE_MAJVER            1 // major file version
#define FILE_MINVER            0 // minor file version
#define FILE_BUILD             _FILE_VERSION_BUILD // build number
#define EXE_YEAR               2014 // current year or timespan (e.g. 2003-2009)
#define TEXT_WEBSITE           http://assarbad.net // website
#define TEXT_PRODUCTNAME       Some Win32 CUI application // product's name
#define TEXT_FILEDESC          This is the CUI application to my very cool product // component description
#define TEXT_COMPANY           assarbad.net // company
#define TEXT_MODULE            runasinvoker // module name
#define TEXT_COPYRIGHT         Copyright \xA9 EXE_YEAR TEXT_COMPANY // copyright information
// #define TEXT_SPECIALBUILD      // optional comment for special builds
#define TEXT_INTERNALNAME      runasinvoker.exe // copyright information
// #define TEXT_COMMENTS          // optional comments
// ---------------------------------------------------------------------------
// ... well, that's it. Pretty self-explanatory ;)
// ---------------------------------------------------------------------------

#endif // __EXEVERSION_H_VERSION__
