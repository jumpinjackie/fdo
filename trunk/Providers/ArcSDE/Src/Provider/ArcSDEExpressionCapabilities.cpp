/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */

#include "stdafx.h"
#include <FdoExpressionEngine.h>

ArcSDEExpressionCapabilities::ArcSDEExpressionCapabilities ()
{
}

ArcSDEExpressionCapabilities::~ArcSDEExpressionCapabilities ()
{
}

void ArcSDEExpressionCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of FdoExpressionType objects the feature provider supports.</summary>
/// <param name="length">Input the number of expression types</param> 
/// <returns>Returns the list of expression types</returns> 
FdoExpressionType* ArcSDEExpressionCapabilities::GetExpressionTypes (FdoInt32& length)
{
    static FdoExpressionType types[] =
    {
      FdoExpressionType_Basic,
      FdoExpressionType_Function  // Aggregate functions only at this point
// NOT IN R1: FdoExpressionType_Parameter
    };

    length = sizeof (types) / sizeof (FdoExpressionType);

    return (types);
}

/// <summary>Returns a collection of FdoFunctionDefinition objects the feature provider supports within expressions.</summary>
/// <returns>Returns the collection of function definitions</returns> 
FdoFunctionDefinitionCollection* ArcSDEExpressionCapabilities::GetFunctions()
{
    if (NULL == m_supportedFunctions)
    {
		m_supportedFunctions = FdoExpressionEngine::GetStandardFunctions();
    }

    return (FDO_SAFE_ADDREF (m_supportedFunctions.p));
}

