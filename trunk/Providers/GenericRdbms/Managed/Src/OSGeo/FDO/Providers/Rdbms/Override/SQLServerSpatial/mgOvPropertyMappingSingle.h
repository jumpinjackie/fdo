//
// Copyright (C) 2004-2008  Autodesk, Inc.
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

#pragma once

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgIOvPropertyMappingDefinition.h"

class FdoSqlServerOvPropertyMappingSingle;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
ref class OvClassDefinition;

///<summary>Abstract class defining the physical schema overrides for relation type object property
///table mappings.</summary>
public ref class OvPropertyMappingSingle : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingSingle, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingDefinition
{
public:
	///<summary>Constructs a default of an FdoSqlServerOvPropertyMappingSingle</summary>
	OvPropertyMappingSingle();

internal:
	OvPropertyMappingSingle(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvPropertyMappingSingle* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
