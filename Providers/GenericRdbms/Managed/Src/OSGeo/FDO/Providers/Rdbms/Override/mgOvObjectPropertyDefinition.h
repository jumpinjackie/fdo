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

class FdoRdbmsOvObjectPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
ref class XmlSaxContext;
ref class XmlAttributeCollection;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

ref class OvPropertyMappingDefinition;
ref class OvPropertyMappingSingle;
ref class OvPropertyMappingConcrete;

///<summary>Abstract class defining physical schema overrides for an object property
///definition.</summary>
public ref class OvObjectPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition
{
internal:
	inline FdoRdbmsOvObjectPropertyDefinition* GetImpObj();

public protected:
	OvObjectPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    ///<summary>Gets the object property mapping definition</summary>
    /// <returns>Returns OvPropertyMappingDefinition</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition^ MappingDefinition
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition^ get();
    }

   // Initialize this element from its XML attributes
	System::Void InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes, System::String^ mappingType, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ mappingAttributes);

    // Create and optionally attach a Single table property mapping.
	NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingSingle^ CreateSingleMapping(System::Boolean attach);

    // Create and optionally attach a Class table property mapping.
	NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass^ CreateClassMapping(System::Boolean attach);

    // Create and optionally attach a Concrete table property mapping.
	NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingConcrete^ CreateConcreteMapping(System::Boolean attach);
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
