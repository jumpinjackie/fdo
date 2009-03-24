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

#pragma once

#include "mgIOvPropertyMappingRelation.h"

class FdoRdbmsOvPropertyMappingRelation;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

ref class OvClassDefinition;

///<summary>
///Abstract class defining the physical schema overrides for relation type 
///object property table mappings.
///</summary>
public ref class OvPropertyMappingRelation : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::IOvPropertyMappingRelation
{	
internal:
	FdoRdbmsOvPropertyMappingRelation* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
	
public protected:
	OvPropertyMappingRelation(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    ///<summary>Gets the internal class</summary>
    /// <returns>Returns the internal class</returns>
    virtual property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ InternalClass
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ get();
    }

	// Creates and optionally attaches an internal class override object.
	virtual NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ CreateInternalClass(System::Boolean attach);

};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

