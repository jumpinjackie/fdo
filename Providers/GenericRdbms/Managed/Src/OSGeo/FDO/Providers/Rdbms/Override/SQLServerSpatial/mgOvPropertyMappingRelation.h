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
#pragma warning (disable:4490)

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgIOvPropertyMappingRelation.h"

class FdoSqlServerOvPropertyMappingRelation;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
ref class OvClassDefinition;

#pragma warning(push)
#pragma warning(disable: 4490)  //  warning C4490: 'new' : incorrect use of override specifier;

///<summary>Abstract class defining the physical schema overrides for relation type object property
///table mappings.</summary>
public ref class OvPropertyMappingRelation : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingRelation, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingRelation
{
private:
    virtual property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ InternalClass2
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ get()  sealed = NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::IOvPropertyMappingRelation::InternalClass::get
        {
            return __super::InternalClass;
        }
    }
public:
    ///<summary>Gets the internal class</summary>
    /// <returns>Returns the internal class</returns>
    ///<summary>Sets the internal class</summary>
    /// <returns>Returns nothing</returns>
    virtual property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition^ InternalClass
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition^ get() new sealed = NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingRelation::InternalClass::get;
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition^ classDefinition) new sealed = NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingRelation::InternalClass::set;
    }

internal:
	OvPropertyMappingRelation(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvPropertyMappingRelation* GetImpObj();
};

#pragma warning(pop)  //  warning C4490: 'new' : incorrect use of override specifier;

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
