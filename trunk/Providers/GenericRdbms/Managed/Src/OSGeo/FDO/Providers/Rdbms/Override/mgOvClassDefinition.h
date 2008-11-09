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

class FdoRdbmsOvClassDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlWriter;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFlags;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

public __gc class OvReadOnlyPropertyDefinitionCollection;
public __gc class OvTable;
public __gc class OvGeometricColumn;

///<summary>Abstract class defining physical schema overrides for a class definition.</summary>
public __gc class OvClassDefinition: public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping
{
public private:
	FdoRdbmsOvClassDefinition* GetImpObj();
	
public protected:
	OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    ///<summary>Gets the collection of property overrides for this class
    ///override.</summary>
    /// <returns>Returns OvReadOnlyPropertyDefinitionCollection</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection* get_Properties();

    ///<summary>Gets the table overrides for this class override</summary>
    /// <returns>Returns OvTable</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable* get_Table();

    ///<summary>Gets the table mapping</summary>
    ///<returns>Returns OvTableMappingType</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType get_TableMapping();

    ///<summary>Sets the table mapping</summary>
    /// <param name="mappingType">Input the mapping type</param>
    ///<returns>Returns nothing</returns>
	__property System::Void set_TableMapping(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType mappingType);

	__property System::Void set_Parent(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* value);
     
    // Serialize the XML attributes of this class:
	System::Void WriteXmlAttributes(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

