#
# Copyright (C) 2004-2007  Autodesk, Inc.
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser
# General Public License as published by the Free Software Foundation.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

'FDO Python Wrappers'

This directory contains the code for building the Python wrappers for FDO.

Directory contents:
-----------------------------------------------------------------------------

Files:
	build.cmd             (Win32) Build script. Builds wrapper for FDO API and all overrides
	PythonWrappers.sln    (Win32) Visual Studio .NET solution file for all wrappers
	Fdo*.vcproj           (Win32) Visual Studio .NET project files	
	
Directories:
	/Inc                  C++ Header files for the wrapper utilities
	
	/Fdo_Inc	          Modified FDO C++ header files.  SWIG has problems with some
	                      C++ constructs used in FDO.  /Fdo_Inc contains modified
	                      versions of the header files to generate the wrappers.
	                      
	/Inc_Merged           Temporary directory created by build scripts.  A merge of
	                      the header files in FDO SDK, and those in /fdo_inc
	                      
	/Lib                  Temporary directory created by Visual Studio. Target directory
	                      of the build process.
	                      
	/Resource             Resource files for Win32
	
	/Src                  C++ source files for the wrapper utilities
	
	/Swig                 SWIG interface files
		/Common           SWIG interface files shared for all wrappers
		/Fdo              SWIG interface files for FDO API
		
	/UnitTests            Python unit tests for the FDO API wrapper


