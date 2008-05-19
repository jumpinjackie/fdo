#ifndef FDOSMLPSPATIALCONTEXT_H
#define FDOSMLPSPATIALCONTEXT_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/Schema.h>
#include <Sm/Lp/SchemaElement.h>
#include <Sm/Ph/SpatialContextReader.h>
#include <Sm/Ph/SpatialContextWriter.h>
#include <Sm/Ph/SpatialContextGroupReader.h>
#include <Sm/Ph/SpatialContextGroupWriter.h>

// This logical/physical class encapsulates physical interactions with
// Spatial Contexts for callers at the logical or logical/physical level.
// It includes the merging of publicly-visible Spatial Contexts with
// the non-public Spatial Context Groups.
class FdoSmLpSpatialContext : public FdoSmLpSchemaElement
{
	friend class FdoSmLpSpatialContextCollection;
	friend class FdoRdbmsOptimizedAggregateReader;

public:
    ~FdoSmLpSpatialContext(void);

    /// Gets the internal datastore Spatial Context ID number. This id does not get
    /// exposed through FDO.
	FdoInt64 GetId() const;

    /// Accessors for publicly-accessible attributes.
    FdoString* GetCoordinateSystem();
    FdoString* GetCoordinateSystemWkt();
    FdoSpatialContextExtentType GetExtentType();
    FdoByteArray* GetExtent();
    double GetXYTolerance();
    double GetZTolerance();

    /// Get the SRID.
    FdoInt64 GetSrid();

    /// Set the modification state of this element
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Post outstanding modifications to the current datastore.
	virtual void Commit( bool fromParent = false );

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Construct an instance from readers
    FdoSmLpSpatialContext(
        FdoSmPhSpatialContextReaderP scReader,
        FdoSmPhSpatialContextGroupReaderP scgReader,
        FdoSmPhMgrP physicalSchema);
	

    /// Constructs an instance from the given attributes.
	FdoSmLpSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
        FdoSmPhMgrP physicalSchema
    );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmLpSpatialContext() {}

    /// Get a writer for adding a spatial context.
    /// Returns a writer with fields already set.
    virtual FdoSmPhSpatialContextWriterP GetPhysicalScAddWriter();

    /// Get a writer for modifying a spatial context.
    /// Returns a writer with fields already set.
    virtual FdoSmPhSpatialContextWriterP GetPhysicalScModifyWriter();

    /// Get a writer for adding a spatial context group.
    /// Returns a writer with fields already set.
    virtual FdoSmPhSpatialContextGroupWriterP GetPhysicalScgAddWriter();

    /// Get a writer for modifying a spatial context group.
    /// Returns a writer with fields already set.
    virtual FdoSmPhSpatialContextGroupWriterP GetPhysicalScgModifyWriter();

    /// Get the Physical Schema Manager.
    FdoSmPhMgrP GetPhysicalSchema();

    /// Get WKT without tripping Finalize
    FdoString* Get_CoordinateSystemWkt();

    /// Set the ID.
    void SetId( FdoInt64 id );

    /// Set the name.
    void SetCoordinateSystemName( FdoString* name );

    /// Set the WKT.
    void SetCoordinateSystemWkt( FdoString* wkt );

    /// Set the SRID.
    void SetSrid( FdoInt64 srid );

	/// Get a matching spatial context group definition.
	FdoInt64  GetMatchingScgid();

    // The following add or delete the spatial context in the datastore, when 
    // the datastore has no MetaSchema.
    virtual void AddNoMeta();
    virtual void DeleteNoMeta();

    /// Finalize this spatial context.
	virtual void Finalize();

private:
    void AddNoMetaNameChangeError( FdoSmPhOwnerP owner );
    void AddNoMetaNameLengthError( FdoSmPhOwnerP owner, FdoSize maxLen );
    void AddNoCsysError();
    void AddNoWktError();
	void AddMismatchedWktError();

	FdoSmPhMgrP                 mPhysicalSchema;
    FdoInt64                    mId;            // Spatial Context ID
    FdoInt64                    mScgId;         // Spatial Context Group ID
    FdoStringP                  mCoordSysName;
    FdoStringP                  mCoordSysWkt;

    FdoSpatialContextExtentType mExtentType;
    FdoPtr<FdoByteArray>        mExtent;

    double                      mXYTolerance;
    double                      mZTolerance;

protected:
	FdoInt64                    mSrid;			// Coordinate system SRID. 
};

typedef FdoPtr<FdoSmLpSpatialContext> FdoSmLpSpatialContextP;

#endif


