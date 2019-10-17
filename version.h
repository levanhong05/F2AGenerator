/****************************************************************************
**
** Copyright (C) 2019 Eric Lee.
** Contact: levanhong05@gmail.com
** Company: Freelancer
**
** This file is part of the F2A Generator project.
**
**All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
**
****************************************************************************/

#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR               "1"
#define VERSION_MINOR               "0"
#define VERSION_REVISION            "0"
#define BUILD_NUMBER                "1"

#define RELEASENUMBER               1

#define VER_PRODUCTVERSION_STR      VERSION_MAJOR "." VERSION_MINOR
#define VER_FILEVERSION_STR         VERSION_MAJOR "." VERSION_MINOR "." VERSION_REVISION
#define VER_FILEVERSIONFULL_STR     VERSION_MAJOR "." VERSION_MINOR "." VERSION_REVISION "." BUILD_NUMBER

#define VER_FILEVERSION             1,0,0
#define VER_PRODUCTVERSION          1,0,0

#define VER_COMPANYNAME_STR         "Eric Lee"
#define VER_FILEDESCRIPTION_STR     "F2A Generator Application"
#define VER_INTERNALNAME_STR        "F2A Generator"
#define VER_LEGALCOPYRIGHT_STR      "Copyright (C) 2019 " VER_COMPANYNAME_STR
#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#define VER_ORIGINALFILENAME_STR    "F2AGenerator.exe"
#define VER_PRODUCTNAME_STR         "F2A Generator"
#define VER_MANUFACTURER_STR        "Eric Lee"

#define VER_COMPANYDOMAIN_STR       "https://levanhong05.wordpress.com"
#define VER_PRODUCTDOMAIN_STR       "https://levanhong05.wordpress.com"

#endif // VERSION_H
