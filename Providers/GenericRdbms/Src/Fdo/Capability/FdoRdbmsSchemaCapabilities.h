#ifndef _FDORDBMSSCHEMACAPABILITIES_H_
#define _FDORDBMSSCHEMACAPABILITIES_H_
#ifdef _WIN32
#pragma once
#endif
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
 */


#include "Fdo.h"

class FdoRdbmsSchemaCapabilities :  public FdoISchemaCapabilities
{
public:
    FdoRdbmsSchemaCapabilities(void);

    virtual FdoClassType* GetClassTypes(int& length);

    virtual FdoDataType* GetDataTypes(int& length);

    virtual bool SupportsInheritance();

    virtual bool SupportsMultipleSchemas();

    virtual bool SupportsObjectProperties();

    virtual bool SupportsAssociationProperties();

    virtual bool SupportsSchemaOverrides();

    virtual bool SupportsNetworkModel();

    virtual bool SupportsAutoIdGeneration();

    virtual bool SupportsDataStoreScopeUniqueIdGeneration();

    virtual FdoDataType* GetSupportedAutoGeneratedTypes(FdoInt32& length);

    virtual bool  SupportsSchemaModification();

	/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
	virtual bool SupportsInclusiveValueRangeConstraints();

	/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
	virtual bool SupportsExclusiveValueRangeConstraints();

	/// <summary>Determines if the provider supports value constraints list.</summary>
	/// <returns>Returns true if the provider supports value constraints list.</returns>
	virtual bool SupportsValueConstraintsList();

	/// <summary>Determines if the provider supports nullable value constraint.</summary>
	/// <returns>Returns true if the provider supports nullable value constraint.</returns>
	virtual bool SupportsNullValueConstraints();

	/// <summary>Determines if the provider supports unique value constraint.</summary>
	/// <returns>Returns true if the provider supports unique value constraint.</returns>
	virtual bool SupportsUniqueValueConstraints();

	/// <summary>Determines if the provider supports composite unique value constraint.</summary>
	/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
	virtual bool SupportsCompositeUniqueValueConstraints();

    /// <summary>Determines if the provider supports multiple identity properties per class.</summary>
    /// <returns>Returns true if the provider supports multiple identity properties per class.</returns>
    virtual bool SupportsCompositeId();

protected:
    ~FdoRdbmsSchemaCapabilities(void);
    virtual void Dispose();
};
#endif

