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

class FdoRdbmsOvAssociationPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

///<summary>Abstract class defining physical schema overrides for an association property
///definition.</summary>
public ref class OvAssociationPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition
{
internal:
	inline FdoRdbmsOvAssociationPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

public protected:
	OvAssociationPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
 ///<summary>Gets a collection of Oracle property definitions</summary>
 ///<returns>Returns the collection of Oracle property definitions</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection^ Properties
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection^ get();
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

