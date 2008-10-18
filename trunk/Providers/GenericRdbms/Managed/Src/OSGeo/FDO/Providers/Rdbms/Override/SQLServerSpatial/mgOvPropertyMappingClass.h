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

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgIOvPropertyMappingRelation.h"

class FdoSqlServerOvPropertyMappingClass;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
public __gc class OvClassDefinition;

///<summary>Abstract class defining the physical schema overrides for relation type object property
///table mappings.</summary>
public __gc class OvPropertyMappingClass : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingRelation
{
public:
	///<summary>Constructs a default of an FdoSqlServerOvPropertyMappingClass</summary>
	OvPropertyMappingClass();

    ///<summary>Gets the internal class</summary>
    /// <returns>Returns the internal class</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition* get_InternalClass();

    ///<summary>Sets the internal class</summary>
    /// <returns>Returns nothing</returns>
	__property System::Void set_InternalClass(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition* classDefinition);

public private:
	OvPropertyMappingClass(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvPropertyMappingClass* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
