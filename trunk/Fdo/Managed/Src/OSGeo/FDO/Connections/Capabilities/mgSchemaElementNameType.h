//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#pragma  once

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES
/// \endcond

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \brief
/// The SchemaElementNameType enumeration lists all of the concrete schema name
/// types defined to address schema limitation information.
public __value enum SchemaElementNameType
{
    /// Represents the identifier to address a data store name.
    SchemaElementNameType_Datastore = FdoSchemaElementNameType_Datastore,

    /// Represents the identifier to address a schema.
    SchemaElementNameType_Schema = FdoSchemaElementNameType_Schema,

    /// Represents the identifier to address a class.
    SchemaElementNameType_Class = FdoSchemaElementNameType_Class,

    /// Represents the identifier to address a property.
    SchemaElementNameType_Property = FdoSchemaElementNameType_Property,

    /// Represents the identifier to address a description.
    SchemaElementNameType_Description = FdoSchemaElementNameType_Description,

};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES
/// \endcond
