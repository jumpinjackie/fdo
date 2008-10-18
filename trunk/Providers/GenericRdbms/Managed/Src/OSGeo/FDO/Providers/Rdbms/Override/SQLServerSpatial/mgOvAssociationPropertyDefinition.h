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

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgIOvPropertyDefinition.h"

class FdoSqlServerOvAssociationPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
public __gc class OvPropertyDefinitionCollection;

///<summary>Concrete class defining physical schema overrides for an association property
///definition.</summary>
public __gc class OvAssociationPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvAssociationPropertyDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyDefinition
{
public:
    ///<summary>Constructs a default of an FdoSqlServerOvAssociationPropertyDefinition</summary>
	OvAssociationPropertyDefinition();

    ///<summary>Constructs an instance of an FdoSqlServerOvAssociationPropertyDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvAssociationPropertyDefinition(System::String* name);

	///<summary>Gets a collection of property definitions</summary>
    /// <returns>Returns a collection of property definitions</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinitionCollection* get_IdentityProperties();

public private:
	OvAssociationPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvAssociationPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
