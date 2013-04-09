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
#pragma warning (disable:4490)

class FdoMySQLOvPropertyMappingRelation;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

ref class OvClassDefinition;

///<summary>Abstract class defining the physical schema overrides for relation type object property
///table mappings.</summary>
public ref class OvPropertyMappingRelation : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingRelation, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingRelation
{
private:
    virtual property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ InternalClass2
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ get()  sealed = NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::IOvPropertyMappingRelation::InternalClass::get
        {
            return __super::InternalClass;
        }
    }

internal:
	OvPropertyMappingRelation(System::IntPtr unmanaged, System::Boolean autoDelete);
	FdoMySQLOvPropertyMappingRelation* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

public:
    ///<summary>Gets the internal class</summary>
    /// <returns>Returns the internal class</returns>
    ///<summary>Sets the internal class</summary>
    /// <returns>Returns nothing</returns>
    virtual property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition^ InternalClass
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition^ get() new sealed = NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingRelation::InternalClass::get;
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition^ classDefinition) new sealed = NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingRelation::InternalClass::set;
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL
