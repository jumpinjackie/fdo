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
#ifndef FDOSMLPPOSTGISSCHEMACOLLECTION_H
#define FDOSMLPPOSTGISSCHEMACOLLECTION_H

#include "Schema.h"
#include <Sm/Lp/SchemaCollection.h>

/// Defines LogicalPhysical schema collection for PostGIS provider.
///
class FdoSmLpPostGisSchemaCollection : public FdoSmLpSchemaCollection
{
public:

    /// Create an empty collection.
    FdoSmLpPostGisSchemaCollection();

    /// Create a populated collection.
    ///
    /// \param
    /// mgr [in] - Physical Schema Manager.
    ///
    /// \param
    /// spatialContexts [in] - instance of spatial context.
    ///
    FdoSmLpPostGisSchemaCollection(FdoSmPhMgrP mgr,
        FdoSmLpSpatialContextMgrP scMgr);

    /// \todo To be documented
    ~FdoSmLpPostGisSchemaCollection();

    virtual bool CanApplySchemaWithoutMetaSchema() const;

protected:

    /// Create a new schema from a schema reader.
    /// \todo To be documented
    virtual FdoSmLpSchemaP NewSchema(FdoSmPhSchemaReaderP reader);

	/// Create a new LogicalPhysical feature schema from FDO feature schema.
    /// \todo To be documented
    virtual FdoSmLpSchemaP NewSchema(FdoFeatureSchema* featSchema,
        bool ignoreStates);

};

/// \brief
/// Declaration of smart-pointer type for FdoSmLpPostGisSchemaCollection.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmLpPostGisSchemaCollection> FdoSmLpPostGisSchemasP;

#endif // FDOSMLPPOSTGISSCHEMACOLLECTION_H