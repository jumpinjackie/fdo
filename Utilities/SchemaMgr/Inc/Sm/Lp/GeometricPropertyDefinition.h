#ifndef FDOSMLPGEOMETRICPROPERTYDEFINITION_H
#define FDOSMLPGEOMETRICPROPERTYDEFINITION_H		1
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

#include <Sm/Lp/SimplePropertyDefinition.h>
#include <Utilities/SchemaMgr/Overrides/GeometricColumnType.h>
#include <Utilities/SchemaMgr/Overrides/GeometricContentType.h>


// GeometricPropertyDefinition derives from SimplePropertyDefinition,
// adding the possibility of being stored in ways other than a single
// column, specifically for geometric values.  It supports the overrides
// defined by FdoRdbmsOvGeometricPropertyDefinition.
class FdoSmLpGeometricPropertyDefinition : public FdoSmLpSimplePropertyDefinition
{
public:

  
    /// Get the type of property ( object, data, geometric, etc. ) that
    /// this object represents.
	virtual FdoPropertyType GetPropertyType() const;

    /// Gets the allowable Geometry Types for this property.
    FdoInt32 GetGeometryTypes() const;

    /// Gets binary-or-combined value of the specific geometry types
    FdoInt32 GetSpecificGeometryTypes() const;

    /// Gets a Boolean value that indicates if the geometry of this property
    /// include elevation values.
    bool GetHasElevation() const;

    /// Gets a Boolean value that indicates if the geometry of this property
    /// include measurement values that can be used for dynamic segmentation.
    bool GetHasMeasure() const;

    /// Returns DBI-format column dimensionality, based on whether the column
    /// has elevation or measure dimensions.
    /// See inc/dbi/coord_ty.h for the list of values that can be returned.
//TODO    int GetDimensionality();

    /// \brief
    /// Gets the Spatial Context name associated to this geometric property.
    /// 
    /// \return
    /// Returns a String value representing the Spatial Context name.
    /// If NULL then the geometric property is associated with all the Spatial Contexts
    /// in the datastore (supports multiple geometric representations via this property).
    /// 
    FdoString * GetSpatialContextAssociation() const;

    /// Get the Schema Mappings for this geometric property
	//
    /// Parameters:
    /// 	bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      The Schema Mappings.
    ///      NULL if there are no mappings.
/*
    virtual FdoOracleOvPropertyP GetSchemaMappings( bool bIncludeDefaults ) const;
    
    /// Get the column mapping for this geometric property
	//
    /// Parameters:
    /// 	bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      The column Mapping.
    ///      NULL if there is no column mapping.

    FdoOracleOvGeometricColumnP GetColumnMapping( bool bIncludeDefaults ) const;
*/
    /// Makes this property an inherited property.
	//
    /// Parameters:
    /// 	pBaseProp: the property to inherit from.
	virtual void SetInherited( const FdoSmLpPropertyDefinition* pBaseProp );
/*
    /// Inherit this property to a subclass. Creates a new instance of this 
    /// property, which can be attached to the subclass.
	//
    /// Parameters:
    /// 	pSubClass: inherit to this subclass. Must be a subclass of this
    /// 		property's defining class.
	virtual FdoSmLpPropertyDefinition* CreateInherited( FdoSmLpClassDefinition* pSubClass ) const;

    /// Create a non-inherited copy of this property. 
	//
    /// Parameters:
    /// 	pTargetClass: The class that will contain the new property.
    /// 	logicalName: The name for the new property.
    /// 		If L"" then the new property has the same name as this
    /// 		property.
    /// 	physicalName: column name.
    /// 		If L"" then the column name is set to the base property column name.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
	virtual FdoSmLpPropertyDefinition* CreateCopy( 
        FdoSmLpClassDefinition* pTargetClass, 
        FdoStringP logicalName, 
        FdoStringP physicalName,
        FdoOracleOvPropertyDefinition* pPropOverrides = NULL
    ) const;
*/

    //////////////////////////////////////////////////////////////////////

    /// Replicate some column-oriented accessor and mutator methods
    /// inherited from FdoSmLpSimplePropertyDefinition, but for extra
    /// columns that may be needed for geometry.
    /// We support up to three ordinate columns , and two spatial indexing
    /// "Si1" and "Si2" columns.

	FdoString* GetColumnNameX() const;
	FdoString* GetRootColumnNameX() const;
	const FdoSmPhColumn* RefColumnX() const;
    FdoSmPhColumnP GetColumnX();
    bool GetIsFixedColumnX() const { return mbFixedColumnX; }
    bool GetIsColumnCreatorX() const { return mbColumnCreatorX; }
	void SetColumnNameX( FdoString* columnName );
	void SetRootColumnNameX( FdoString* columnName );
	void SetColumnX( FdoSmPhColumnP pColumn );
    void SetIsFixedColumnX( bool bFixedColumn ) { mbFixedColumnX = bFixedColumn; }
    void SetIsColumnCreatorX( bool bColumnCreator ) { mbColumnCreatorX = bColumnCreator; }

	FdoString* GetColumnNameY() const;
	FdoString* GetRootColumnNameY() const;
	const FdoSmPhColumn* RefColumnY() const;
    FdoSmPhColumnP GetColumnY();
    bool GetIsFixedColumnY() const { return mbFixedColumnY; }
    bool GetIsColumnCreatorY() const { return mbColumnCreatorY; }
	void SetColumnNameY( FdoString* columnName );
	void SetRootColumnNameY( FdoString* columnName );
	void SetColumnY( FdoSmPhColumnP pColumn );
    void SetIsFixedColumnY( bool bFixedColumn ) { mbFixedColumnY = bFixedColumn; }
    void SetIsColumnCreatorY( bool bColumnCreator ) { mbColumnCreatorY = bColumnCreator; }

    FdoString* GetColumnNameZ() const;
	FdoString* GetRootColumnNameZ() const;
	const FdoSmPhColumn* RefColumnZ() const;
    FdoSmPhColumnP GetColumnZ();
    bool GetIsFixedColumnZ() const { return mbFixedColumnZ; }
    bool GetIsColumnCreatorZ() const { return mbColumnCreatorZ; }
	void SetColumnNameZ( FdoString* columnName );
	void SetRootColumnNameZ( FdoString* columnName );
	void SetColumnZ( FdoSmPhColumnP pColumn );
    void SetIsFixedColumnZ( bool bFixedColumn ) { mbFixedColumnZ = bFixedColumn; }
    void SetIsColumnCreatorZ( bool bColumnCreator ) { mbColumnCreatorZ = bColumnCreator; }

	FdoString* GetColumnNameSi1() const;
	FdoString* GetRootColumnNameSi1() const;
	const FdoSmPhColumn* RefColumnSi1() const;
    FdoSmPhColumnP GetColumnSi1();
    // SI column fixed name and FDO ownership status not tracked in MetaSchema.
    // However, these are in lock step with equivalent statuses for geometric column.
    bool GetIsFixedColumnSi1() const { return GetIsFixedColumn(); }
    bool GetIsColumnCreatorSi1() const { return GetIsColumnCreator(); }
	void SetColumnNameSi1( FdoString* columnName );
	void SetRootColumnNameSi1( FdoString* columnName );
	void SetColumnSi1( FdoSmPhColumnP pColumn );

	FdoString* GetColumnNameSi2() const;
	FdoString* GetRootColumnNameSi2() const;
	const FdoSmPhColumn* RefColumnSi2() const;
    FdoSmPhColumnP GetColumnSi2();
    // SI column fixed name and FDO ownership status not tracked in MetaSchema.
    // However, these are in lock step with equivalent statuses for geometric column.
    bool GetIsFixedColumnSi2() const { return GetIsFixedColumn(); }
    bool GetIsColumnCreatorSi2() const { return GetIsColumnCreator(); }
	void SetColumnNameSi2( FdoString* columnName );
	void SetRootColumnNameSi2( FdoString* columnName );
	void SetColumnSi2( FdoSmPhColumnP pColumn );

    //////////////////////////////////////////////////////////////////////

    FdoSmOvGeometricColumnType GetGeometricColumnType() const
    {
        return mGeometricColumnType;
    }
    void SetGeometricColumnType(FdoSmOvGeometricColumnType columnType)
    {
        mGeometricColumnType = columnType;
    }
    FdoSmOvGeometricContentType GetGeometricContentType() const
    {
        return mGeometricContentType;
    }
    void SetGeometricContentType(FdoSmOvGeometricContentType contentType)
    {
        mGeometricContentType = contentType;
    }

    /// Convenience function for casting a property to a geometric property.
    /// Returns NULL if the property is not a geometric property.
	static const FdoSmLpGeometricPropertyDefinition* Cast( const FdoSmLpPropertyDefinition* src )
	{
		return( src ? ( src->GetPropertyType() == FdoPropertyType_GeometricProperty ? 
						(const FdoSmLpGeometricPropertyDefinition*) src  : NULL 
					  ) : NULL );
	}

    // Change the primary geometry status
    //
    // Parameters
    //      isPrimary:
    //          true: this property is the main geometry for its feature class
    //          false: this property is not the main geometry
	void SetPrimary( bool isPrimary );

    /// Copy updates from corresponding FDO property. 
    //
    /// Parameters
    ///      pFdoProp: the FDO element.
    ///      elementState: the modification state for this data property.
    ///      pPropOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO element is 
    ///          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates
    ); 
    /// Synchronize this property and its column. Creates a column if the property does
    /// not have one. 
    //
    /// Parameters:
    //
    ///      bRollbackOnly - 
    ///          true - synchronize only if a change to this property has been rolled back since
    ///              the previous synchronization.
    ///          false - always synchronize this property.
	virtual void SynchPhysical(bool bRollbackOnly = true);

    /// Set the modification state of this element
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Post the modifications to this element to the current database.
    virtual void Commit( bool fromParent = false );

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Serialize this property to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;
/*
    /// Geometric column type name
	static const wchar_t* mGeometryColType;
*/
    /// This function checks if the inserted geometry is allowed to be inserted.
    virtual bool CheckGeomPropShapeType (FdoGeometryType geomType) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpGeometricPropertyDefinition() {}

    /// Finalize this property by generating its column.
    virtual void Finalize();

    /// Constructs a new column for this geometric property.
    //
    /// Parameters:
    /// 	dbObject: put the column in this database object.
    /// 	columnName: the name for the new column.
	virtual FdoSmPhColumnP NewColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName );

    bool TableHasSpatialIndexColumns();

    // Find Si column 1 or 2, as determined by the suffix
    FdoSmPhColumnP FindSiColumn( FdoStringP suffix );
    // Given a suffix, generate full name for an SI column
    virtual FdoStringP GenSiColumnName( FdoStringP suffix );
    // Find a column in the property's containing dbObject, without
    // tripping a finalize of this property.
    FdoSmPhColumnP FindColumn( FdoStringP columnName );

    virtual FdoSmPhColumnP NewSiColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName, bool isNullable );
	virtual FdoSmPhColumnP NewOrdColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName, bool isNullable );

    void SetWantSiColumns(bool wantSiColumns) { mWantSiColumns = wantSiColumns; }
    void AddSiColumns();
    virtual FdoSmPhColumnP AddSiColumn( FdoSmPhDbObjectP pPhDbObject, FdoStringP suffix ); 

    FdoSmLpGeometricPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);
    
    FdoSmLpGeometricPropertyDefinition(
        FdoGeometricPropertyDefinition* pFdoProp, 
        FdoString * columnNameX,
        FdoString * columnNameY,
        FdoString * columnNameZ,
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent);

    FdoSmLpGeometricPropertyDefinition(
        FdoGeometricPropertyDefinition* pFdoProp, 
//        FdoOracleOvPropertyDefinition* pPropOverrides,
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent);

    /// Constructs an instance of a GeometricPropertyDefinition from the given parameters.
    /// Used mainly by CreateInherited() and CreateCopy().
    //
    /// Parameters:
    /// 	pBaseProperty: initialize the new property from this base property.
    /// 	pTargetClass: the class that will contain the new property.
    /// 	logicalName: The name for the new property.
    /// 		If L"" then the new property has the same name as this
    /// 		property.
    /// 	physicalName: column name.
    /// 		If L"" then the column name is set to the base property column name.
    /// 	bInherit:
    /// 		True: the new property inherits from the base property.
    /// 		False: the new property is just a copy of the base property.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
	FdoSmLpGeometricPropertyDefinition(
        FdoPtr<FdoSmLpGeometricPropertyDefinition> pBaseProperty, 
		FdoSmLpClassDefinition* pTargetClass, 
		FdoStringP logicalName, 
		FdoStringP physicalName, 
		bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
	);

    void AddOverrideConflictError();

    FdoSmPhScInfoP CreateSpatialContextInfo();

private:
/*
    FdoStringP  GetActiveSpatialContextName();
*/
    void FixSpatialContextAssociation();
    bool CheckSupportedGeometricTypes( FdoGeometricPropertyDefinition* pFdoGeomProp );
    bool CheckSupportedGeometryTypes( FdoGeometricPropertyDefinition* pFdoGeomProp );

	void AddGeometricTypeChangeError( FdoInt32 newTypes, bool bEmptyGeom );
	void AddGeometryTypeChangeError( FdoInt32 newTypes, bool bEmptyGeom );
    void AddHasElevationChangeError();
    void AddAssociatedSCChangeError();
    void AddDimensionalityError();
    void AddSCNotFoundError();
	void AddNoSCFoundError();
    void AddSCTableNotFoundError();

    FdoInt32 mGeometricTypes;
    FdoInt32 mGeometryTypes;

	bool mbHasElevation;

    bool mbHasMeasure;

	FdoStringP      mAssociatedSCName;
	FdoInt64        mAssociatedScId;

    bool mWantSiColumns;

    FdoSmOvGeometricColumnType  mGeometricColumnType;
    FdoSmOvGeometricContentType mGeometricContentType;

	FdoStringP mColumnNameX;
    FdoStringP mRootColumnNameX;
    FdoStringP mCandRootColumnNameX;
	FdoSmPhColumnP mColumnX;
    bool mbFixedColumnX;
    bool mbColumnCreatorX;

	FdoStringP mColumnNameY;
    FdoStringP mRootColumnNameY;
    FdoStringP mCandRootColumnNameY;
	FdoSmPhColumnP mColumnY;
    bool mbFixedColumnY;
    bool mbColumnCreatorY;

	FdoStringP mColumnNameZ;
    FdoStringP mRootColumnNameZ;
    FdoStringP mCandRootColumnNameZ;
	FdoSmPhColumnP mColumnZ;
    bool mbFixedColumnZ;
    bool mbColumnCreatorZ;

	FdoStringP mColumnNameSi1;
    FdoStringP mRootColumnNameSi1;
    FdoStringP mCandRootColumnNameSi1;
	FdoSmPhColumnP mColumnSi1;

	FdoStringP mColumnNameSi2;
    FdoStringP mRootColumnNameSi2;
    FdoStringP mCandRootColumnNameSi2;
	FdoSmPhColumnP mColumnSi2;

    bool mbIsPrimary;
};

typedef FdoPtr<FdoSmLpGeometricPropertyDefinition> FdoSmLpGeometricPropertyP;

#endif


