/*
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

#ifndef FDORDBMSMYSQLSCHEMACAPABILITIES_H
#define FDORDBMSMYSQLSCHEMACAPABILITIES_H        1

#ifdef _WIN32
#pragma once
#endif

#include <../../Src/Fdo/Capability/FdoRdbmsSchemaCapabilities.h>

class FdoRdbmsMySqlSchemaCapabilities :    public FdoRdbmsSchemaCapabilities
{
public:
    FdoRdbmsMySqlSchemaCapabilities(void) 
	{
	}

	/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
	virtual bool SupportsInclusiveValueRangeConstraints()
	{
		return false;
	}

	/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
	virtual bool SupportsExclusiveValueRangeConstraints()
	{
		return false;
	}

	/// <summary>Determines if the provider supports value constraints list.</summary>
	/// <returns>Returns true if the provider supports value constraints list.</returns>
	virtual bool SupportsValueConstraintsList()
	{
		return false;
	}

};

#endif // FDORDBMSMYSQLSCHEMACAPABILITIES_H
