// 
// Copyright (C) 2006 Refractions Research, Inc.
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
#ifndef FDOSMPOSTGISSCHEMAMANAGER_H
#define FDOSMPOSTGISSCHEMAMANAGER_H

#include "../../SchemaMgr/SchemaManager.h"

/// \brief
///  Schema Manager for PostGIS Provider.
///
class FdoPostGisSchemaManager : public FdoGrdSchemaManager
{
public:

    /// Default constructor.
    FdoPostGisSchemaManager();

    /// Overloaded constructor.
    /// @param connection [in] - connection with access to PostGIS-enabled RDBMS.
    /// @param schemaName [in] - default physical schema for current PostGIS connection.
    ///
    FdoPostGisSchemaManager(GdbiConnection* connection, FdoStringP schemaName);

protected:

    /// Destructor
    virtual ~FdoPostGisSchemaManager(void);

    /// Create collection of LogicalPhysical elements for PostGIS Provider.
    /// @param physMgr - 
    /// @param spatialContexts - 
    /// @return
    ///  \todo To be documented
    ///
    virtual FdoSmLpSchemasP CreateLogicalPhysicalSchemas(FdoSmPhMgrP physMgr,
            FdoSmLpSpatialContextMgrP scMgr);

    /// Create Physical Schema Manager for PostGIS provider.
    /// @return
    virtual FdoSmPhMgrP CreatePhysicalSchema();

private:

    FdoStringP mPostGisSchemaName;

};

#endif // FDOSMPOSTGISSCHEMAMANAGER_H
