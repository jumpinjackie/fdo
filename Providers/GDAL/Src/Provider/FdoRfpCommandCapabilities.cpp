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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpCommandCapabilities.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpCommandCapabilities.h"

FdoRfpCommandCapabilities::FdoRfpCommandCapabilities()
{
}

FdoRfpCommandCapabilities::~FdoRfpCommandCapabilities()
{
}

int* FdoRfpCommandCapabilities::GetCommands(int& size)
{
	static const int commands[] = 
		{
			FdoCommandType_Select,
			FdoCommandType_SelectAggregates,
	        FdoCommandType_DescribeSchema,
			FdoCommandType_DescribeSchemaMapping,
		    FdoCommandType_GetSpatialContexts
	    };

	size = sizeof(commands)/sizeof(int);
	return (int*) commands;
}

bool FdoRfpCommandCapabilities::SupportsParameters()
{
	return false;
}

bool FdoRfpCommandCapabilities::SupportsTimeout()
{
	return false;
}

void FdoRfpCommandCapabilities::Dispose()
{
	delete this;
}
