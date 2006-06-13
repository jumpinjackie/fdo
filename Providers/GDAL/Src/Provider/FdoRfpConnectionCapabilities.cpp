/*
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnectionCapabilities.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

 #include "FDORFP.h"
 #include "FdoRfpConnectionCapabilities.h"

FdoRfpConnectionCapabilities::FdoRfpConnectionCapabilities()
{
}

FdoRfpConnectionCapabilities::~FdoRfpConnectionCapabilities()
{
}

FdoThreadCapability FdoRfpConnectionCapabilities::GetThreadCapability()
{
	return FdoThreadCapability_SingleThreaded;
}

FdoSpatialContextExtentType* FdoRfpConnectionCapabilities::GetSpatialContextTypes(int& length)
{
	static FdoSpatialContextExtentType types[] = {FdoSpatialContextExtentType_Static};
	length = sizeof(types) / sizeof(FdoSpatialContextExtentType);
	return types;
}

bool FdoRfpConnectionCapabilities::SupportsLocking()
{
	return false;
}

FdoLockType* FdoRfpConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    size = 0;
    return NULL;
}

bool FdoRfpConnectionCapabilities::SupportsTimeout()
{
	return false;
}

bool FdoRfpConnectionCapabilities::SupportsTransactions()
{
	return false;
}

bool FdoRfpConnectionCapabilities::SupportsLongTransactions()
{
	return false;
}

bool FdoRfpConnectionCapabilities::SupportsSQL()
{
	return false;
}

void FdoRfpConnectionCapabilities::Dispose()
{
	delete this;
}
bool FdoRfpConnectionCapabilities::SupportsConfiguration()
{
	return true;
}

bool FdoRfpConnectionCapabilities::SupportsMultipleSpatialContexts()
{
	return true;
}

bool FdoRfpConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
	return false;
}
