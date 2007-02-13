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

#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"

class FdoPhysicalSchemaMapping;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \brief
/// PhysicalSchemaMapping is an  abstract class that is the base class of
/// all Schema Override sets. Each instance contains the overrides for a 
/// particular Feature Schema and FDO Provider.
/// Each FDO Provider, that allows Schema Overrides, must create a sub-class of this 
/// class. This sub-class must implement the overrides that are specific to the 
/// provider. The Provider can also add support, for serializing to an XML document, 
/// by overriding the functions inherited from XmlSerializable.
public __gc class PhysicalSchemaMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
    /// \brief
    /// Gets the name of the applicable FDO provider.
    /// 
    /// \return
    /// Returns the name of FDO provider to which this Schema Override set 
    /// applies. The provider name must conform to the format:
    /// "[Company].[Provider].[Version]".
    /// 
    __property System::String* get_Provider();

public protected:
    PhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
protected:
    PhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* schemaMapping, System::Boolean autoDelete);
/// \endcond

public private:
    inline FdoPhysicalSchemaMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


