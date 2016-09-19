========================================================================
    CONSOLE APPLICATION : ProviderCapabilities Project Overview
========================================================================

This project demonstrates how to access and display the capabilities FDO
providers. This depends on the existance of a properly registered provider.

The output executable is created in the SDK bin directory in order
for the executable to find the FDO.dll.
An alternative mechanism is to add the SDK bin directory to the system path.

NOTE: The C++ language option "Treat wchar_t as Built-In Type"
must be set to Yes (/Zc:wchar_t). See Project Properties, C/C++, Language.

Operation of the program relies on the proper installation and
registration of one or more FDO providers.

ProviderCapabilities.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

ProviderCapabilities.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named ProviderCapabilities.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
