//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_SCHEMADESCRIPTION_H_INCLUDED
#define FDOPOSTGIS_SCHEMADESCRIPTION_H_INCLUDED

#include "Connection.h"
#include "SpatialContextCollection.h"
#include <PostGIS/Override/PhysicalSchemaMapping.h>
// boost
#include <boost/noncopyable.hpp>

namespace fdo { namespace postgis {

// Forward declarations
class Connection;
class PgGeometryColumn;
class SpatialContext;

/// This is a composite of Logical and Physical Schema details
/// generated from PostGIS-enabled spatial tables.
/// Every spatial table in a currently connected FDO datastore is
/// mapped to a Feature Class.
///
class SchemaDescription :
    public FdoIDisposable,
    private boost::noncopyable
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<SchemaDescription> Ptr;
    
    /// Named constructor used to create new instance of the class.
    static SchemaDescription* Create();
    
    // \todo TODO: Add support of describing selected schema instead of all
    //
    // schemaName [in] - PARAMETER NOT USED. The PostGIS provider uses one hardcoded
    // default schema name: FdoPostGIS.
    //
    void DescribeSchema(Connection* conn, FdoString* schemaName);

    /// Get flag indicating if schema has been successfully described.
    bool IsDescribed() const;

    void ResetSchema();
    
    void SetLogicalSchemas(FdoFeatureSchemaCollection* logicalSchemas);
    void SetSchemaMapping(ov::PhysicalSchemaMapping* phSchemaMapping);
    void SetSpatialContexts(SpatialContextCollection* spContexts);

    FdoFeatureSchemaCollection* GetLogicalSchemas() const;
    ov::PhysicalSchemaMapping* GetSchemaMapping() const;
    SpatialContextCollection* GetSpatialContexts() const;
    
    FdoClassDefinition* FindClassDefinition(FdoIdentifier* id);
    ov::ClassDefinition* FindClassMapping(FdoIdentifier* id);
    
protected:

    /// Default constructor is not used directly.
    SchemaDescription();
    
    // Destructor.
    virtual ~SchemaDescription();
    
    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

private:

    //
    // Private data members
    //

    // Collection of Feature Schemas read from currently connected datastore.
    FdoPtr<FdoFeatureSchemaCollection> mLogicalSchemas;
    
    ov::PhysicalSchemaMapping::Ptr mSchemaMapping;
    
    SpatialContextCollection::Ptr mSpatialContexts;
    
    bool mIsDescribed;
    
    //
    // Private operations
    //

    SpatialContext* CreateSpatialContext(Connection* conn, FdoStringP name,
        PgGeometryColumn* column);

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SCHEMADESCRIPTION_H_INCLUDED
