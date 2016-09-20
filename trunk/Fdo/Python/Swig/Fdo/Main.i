//
// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// -------------------------------------------------------------
//	Includes for the wrapper fdordbms_wrap.cpp file
//	Description:	Insert these preprocessor commands at the
//					beginning of the wrapper file.
// -------------------------------------------------------------

%{
	#include "Fdo/PhysicalSchemaMappingCasts.h"
%}

// -------------------------------------------------------------
//  Description:	Include the external SWIG modules
//
//  Notes:			The .i and .h files MUST be included
//					in the order that is listed below.  This
//					is very important for the SWIG code generator
//					to read the definitions in the correct order.
// -------------------------------------------------------------
//
 
// Define the common initialization routines for all FDO-based wrappers
%include "Common/CommonInit.i"

// Define typemaps, exceptions and pointer casts
%include "Common/CommonTypemaps.i"
%include "Fdo/FdoTypemaps.i"
%include "Common/CommonExceptions.i"
%include "cpointer.i"
%include "Fdo/FdoPointercasts.i"

// Import the GIS and FDO classes
%include "Common/FdowStd.h"
%include "Fdo/CommonIncludes.i"
%include "Fdo/GeometryIncludes.i"
%include "Fdo/FdoIncludes.i"

// Import the utility classes
%include "Common/FdoProxyConnectionManager.h"
%include "Common/StringBuffer.h"
%include "Fdo/PhysicalSchemaMappingCasts.h"

