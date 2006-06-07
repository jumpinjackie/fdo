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

#include "stdafx.h"
#include <Sm/Lp/GeometricPropertyDefinition.h>
#include <Sm/Lp/FeatureClass.h>
#include <Sm/Lp/Schema.h>
#include <Sm/Error.h>
#include <Sm/Ph/PropertyWriter.h>
#include <Sm/Ph/SpatialContextGeomWriter.h>
#include <Sm/Lp/SpatialContextCollection.h>

//FdoString* FdoSmLpGeometricPropertyDefinition::mGeometryColType = L"Geometry";

#define FDOSMLP_SI_COLUMN_LENGTH    255
#define FDOSMLP_SI_COLUMN_1_NAME    L"_SI_1"
#define FDOSMLP_SI_COLUMN_2_NAME    L"_SI_2"
#define FDOSMLP_SI_COLUMN_IDX_SUFFIX    L"_IDX"

FdoSmLpGeometricPropertyDefinition::FdoSmLpGeometricPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent
) : 
	FdoSmLpSimplePropertyDefinition(propReader, parent),
    mWantSiColumns(false),
    mGeometricColumnType(FdoSmOvGeometricColumnType_Default),
    mGeometricContentType(FdoSmOvGeometricContentType_Default),
    mGeometryTypes(propReader->GetDataType().ToLong()),
	mbHasElevation(propReader->GetHasElevation()),
	mbHasMeasure(propReader->GetHasMeasure()),
    mAssociatedScId(propReader->GetSpatialContextAssociationId()),
    mAssociatedSCName(propReader->GetSpatialContextAssociation())
{
    FixSpatialContextAssociation();
}

FdoSmLpGeometricPropertyDefinition::FdoSmLpGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpSimplePropertyDefinition(pFdoProp, bIgnoreStates, parent),
    mbHasElevation(pFdoProp->GetHasElevation()),
    mbHasMeasure(pFdoProp->GetHasMeasure()),
    mWantSiColumns(false),
    mGeometricColumnType(FdoSmOvGeometricColumnType_Default),
    mGeometricContentType(FdoSmOvGeometricContentType_Default),
	mGeometryTypes(pFdoProp->GetGeometryTypes()),
    mAssociatedScId(-1),
    mAssociatedSCName(pFdoProp->GetSpatialContextAssociation())
{
    FixSpatialContextAssociation();
}

FdoSmLpGeometricPropertyDefinition::FdoSmLpGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* pFdoProp, 
    FdoString * columnNameX,
    FdoString * columnNameY,
    FdoString * columnNameZ,
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpSimplePropertyDefinition(pFdoProp, bIgnoreStates, parent),
    mbHasElevation(pFdoProp->GetHasElevation()),
    mbHasMeasure(pFdoProp->GetHasMeasure()),
    mWantSiColumns(false),
    mGeometricColumnType(FdoSmOvGeometricColumnType_Default),
    mGeometricContentType(FdoSmOvGeometricContentType_Default),
	mGeometryTypes(FdoGeometricType_Point),
    mAssociatedScId(-1),
    mAssociatedSCName(pFdoProp->GetSpatialContextAssociation()),
    mColumnNameX(columnNameX),
    mColumnNameY(columnNameY),
    mColumnNameZ(columnNameZ)
{
    FixSpatialContextAssociation();
}

FdoSmLpGeometricPropertyDefinition::FdoSmLpGeometricPropertyDefinition(
    FdoSmLpGeometricPropertyP pBaseProperty, 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
	FdoSmLpSimplePropertyDefinition(
        FDO_SAFE_ADDREF((FdoSmLpGeometricPropertyDefinition*) pBaseProperty), 
        pTargetClass, 
        logicalName, 
        physicalName, 
        bInherit,
        pPropOverrides
    ),
    mWantSiColumns(false),
    mGeometricColumnType(FdoSmOvGeometricColumnType_Default),
    mGeometricContentType(FdoSmOvGeometricContentType_Default),
	mGeometryTypes( pBaseProperty->GetGeometryTypes() ),
	mbHasElevation( pBaseProperty->GetHasElevation() ),
	mbHasMeasure( pBaseProperty->GetHasMeasure() ),
    mAssociatedScId(-1),
    mAssociatedSCName( pBaseProperty->GetSpatialContextAssociation() )
{
//    Update( pPropOverrides );
    FixSpatialContextAssociation();
}

void FdoSmLpGeometricPropertyDefinition::FixSpatialContextAssociation()
{
    if (mAssociatedSCName.GetLength() <= 0)
    {
        FdoSmLpSchemasP schemas = GetLogicalPhysicalSchema()->GetSchemas();
        FdoSmLpSpatialContextsP scs = schemas->GetSpatialContexts();

        if (scs->GetCount() >= 1)
        {
            // We do not have access to the active spatial context here, so let's
            // at least pick the default one -- it'll always be the first one,
            // because the spatial contexts are read in order of numeric ID.
			// This is true unless the "Default" spatial context was removed/replaced.
            FdoSmLpSpatialContextP sc = scs->GetItem(0);
            mAssociatedSCName = sc->GetName();
            mAssociatedScId = sc->GetId();
        }
    }
    else if (mAssociatedScId < 0)
    {
        FdoSmLpSchemasP schemas = GetLogicalPhysicalSchema()->GetSchemas();
        FdoSmLpSpatialContextsP scs = schemas->GetSpatialContexts();

        FdoSmLpSpatialContextP sc = scs->FindItem(mAssociatedSCName);
        if (sc != NULL)
            mAssociatedScId = sc->GetId();
    }
}
FdoPropertyType FdoSmLpGeometricPropertyDefinition::GetPropertyType() const
{
	return FdoPropertyType_GeometricProperty;
}

FdoInt32 FdoSmLpGeometricPropertyDefinition::GetGeometryTypes() const
{
    return mGeometryTypes;
}

bool FdoSmLpGeometricPropertyDefinition::GetHasElevation() const
{
    return mbHasElevation;
}

bool FdoSmLpGeometricPropertyDefinition::GetHasMeasure() const
{
    return mbHasMeasure;
}

FdoString * FdoSmLpGeometricPropertyDefinition::GetSpatialContextAssociation() const
{
	return mAssociatedSCName;
}

void FdoSmLpGeometricPropertyDefinition::SetInherited( const FdoSmLpPropertyDefinition* pBaseProp )
{
	bool same = true;

	if ( (pBaseProp->GetElementState() != FdoSchemaElementState_Deleted) &&
		 (GetElementState() != FdoSchemaElementState_Deleted) ) {

		const FdoSmLpGeometricPropertyDefinition* pBaseGeomProp = 
			FdoSmLpGeometricPropertyDefinition::Cast( pBaseProp );

		if ( pBaseGeomProp ) {
			if ( mGeometryTypes != pBaseGeomProp->GetGeometryTypes() ) {
				// Inherited property cannot redefine geometry types.
				same = false;
				AddRedefinedError( pBaseGeomProp );
			}
		}		 
	}

	if ( same ) 
		FdoSmLpSimplePropertyDefinition::SetInherited( pBaseProp );
}

void FdoSmLpGeometricPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,       
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
	FdoSmPhMgrP		    pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoSmPhTableP       pPhTable; 

	FdoSmLpSimplePropertyDefinition::Update(pFdoProp, elementState, pPropOverrides, bIgnoreStates);

	// Superclass Update logs property type conflict so just silently skip validation
	// at this level.
	if ( pFdoProp->GetPropertyType() == FdoPropertyType_GeometricProperty ) {

        FdoGeometricPropertyDefinition* pFdoGeomProp = 
			(FdoGeometricPropertyDefinition*) pFdoProp;

		// Handle modifiable settings.
        if ( (GetElementState() == FdoSchemaElementState_Added) || 
            (GetElementState() == FdoSchemaElementState_Modified) ||
            GetIsFromFdo()
        ) {
			SetReadOnly( pFdoGeomProp->GetReadOnly() );
            mbHasElevation = pFdoGeomProp->GetHasElevation();
	        mbHasMeasure = pFdoGeomProp->GetHasMeasure();
            mAssociatedSCName = pFdoGeomProp->GetSpatialContextAssociation();
            FixSpatialContextAssociation();
        }

        if ( (GetElementState() == FdoSchemaElementState_Added) || 
             GetIsFromFdo()
        ) {
#pragma message ("ToDo: Get spatial contexts from config files")
            // Patch for lack of spatial context retrieval from config files
            if ( mAssociatedSCName.ICompare(L"") == 0 )
            {
                mAssociatedSCName = L"Default";
                FixSpatialContextAssociation();
            }
 /* TODO:             
            // If the spatial context was not set then pick up the active one
            if ( mAssociatedSCName.ICompare(L"") == 0 )
            {
                mAssociatedSCName = GetActiveSpatialContextName();
            }
*/
        }
        else if ( GetElementState() == FdoSchemaElementState_Modified ) {
            // Allow under certain circumstances, all errors reported
			if ( mGeometryTypes != pFdoGeomProp->GetGeometryTypes() )
            {
                if ( CheckSupportedGeometricTypes( pFdoGeomProp ) )
                    mGeometryTypes = pFdoGeomProp->GetGeometryTypes();
            }
		}
	}
}

void FdoSmLpGeometricPropertyDefinition::SynchPhysical(bool bRollbackOnly)
{	
	FdoSmPhMgrP pPhysical	        = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoSmPhDbObjectP pPhDbObject	= pPhysical->FindDbObject( GetContainingDbObjectName() );

    // Proceed if synchronizing everything or a change to this property has been rolled back.
	if ( pPhDbObject && (!bRollbackOnly || pPhysical->RefRollbackColumn(pPhDbObject->GetQName(), GetColumnName())) ) {
		
        // Proceed only if the property has a containing table. Can't synchronize column if there is no table.
        // Proceed only if the property has no column.
		if ( !GetColumn() ) {
			if ( wcscmp(RefDefiningClass()->RefLogicalPhysicalSchema()->GetName(), FdoSmLpSchema::mMetaClassSchemaName) != 0 ) {
                if ( GetErrors()->GetCount() == 0 ) {

                    // Proceed only if geometric property is error-free.
                    // Create a column for the property.
				    CreateColumn( pPhDbObject );
                }
			}
		}
	}
}

void FdoSmLpGeometricPropertyDefinition::SetElementState(FdoSchemaElementState elementState)
{
    FdoSmLpSimplePropertyDefinition::SetElementState( elementState );
}

void FdoSmLpGeometricPropertyDefinition::Commit( bool fromParent )
{
	FdoSmLpSimplePropertyDefinition::Commit( fromParent );

	FdoSmPhMgrP	                pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    // Get containing class.
    FdoSmLpClassDefinition*		pClass = (FdoSmLpClassDefinition*) RefParentClass();
	
    // Get the highest level containing class. Different from containing class
	// when this property is nested within an object property.
	FdoSmLpClassDefinition*		pTopClass = (FdoSmLpClassDefinition*)(GetTopProperty()->RefParentClass());
    
    FdoSmPhPropertyWriterP      pWriter = pPhysical->GetPropertyWriter();

    char*						pUser = "fdo_user"; //TODO: pConn->GetUser();
	FdoStringP				    dbUser(pUser);
	
//	delete[] pUser;

	switch ( GetElementState() ) {

    case FdoSchemaElementState_Deleted:  

        // Remove the association with spatial context
        if (mAssociatedScId >= 0) {
            FdoSmPhSpatialContextGeomWriterP scGeomWriter = pPhysical->GetSpatialContextGeomWriter();
            scGeomWriter->Delete( GetContainingDbObjectName(), GetColumnName() );
        }
        break;

	case FdoSchemaElementState_Added:

		// Skip inherited properties not being added to the containing class's table.
        // These will generate a duplicate index error if added.
		if ( !GetBaseProperty() || (FdoStringP(GetContainingDbObjectName()).ICompare(pClass->GetDbObjectName()) == 0) ) {

			pWriter->SetTableName( GetContainingDbObjectName() );
			pWriter->SetClassId( pTopClass->GetId() );
			pWriter->SetColumnName( GetColumnName() );
            pWriter->SetRootObjectName( GetRootColumnName() );
			pWriter->SetName( GetNestedName() );
			pWriter->SetColumnType( GetColumn() ? GetColumn()->GetTypeName() : L"n/a"   );
            pWriter->SetDataType( FdoStringP::Format(L"%d", GetGeometryTypes()) );
			pWriter->SetIsNullable( true );
			pWriter->SetIsFeatId( GetIsFeatId() );
			pWriter->SetIsSystem( GetIsSystem() );
			pWriter->SetIsReadOnly(	GetReadOnly() );
			pWriter->SetUser( dbUser );
			pWriter->SetDescription( GetDescription() );
			pWriter->SetHasElevation( GetHasElevation() );
			pWriter->SetHasMeasure( GetHasMeasure() );
			pWriter->SetIsFixedColumn( GetIsFixedColumn() );
			pWriter->SetIsColumnCreator( GetIsColumnCreator() );
 			pWriter->Add();
            if (mAssociatedScId >= 0) {
                FdoSmPhSpatialContextGeomWriterP scGeomWriter = pPhysical->GetSpatialContextGeomWriter();
                scGeomWriter->SetScId(mAssociatedScId);
                scGeomWriter->SetGeomTableName( GetContainingDbObjectName() );
                scGeomWriter->SetGeomColumnName( GetColumnName() );
                FdoInt32 dimensionality = FdoDimensionality_XY;
                if (GetHasElevation())
                    dimensionality |= FdoDimensionality_Z;
                if (GetHasMeasure())
                    dimensionality |= FdoDimensionality_M;
                scGeomWriter->SetDimensionality( dimensionality );

                // Delete any previous record.  This can happen if multiple classes
                // define geometric properties on the same table/column.  The association
                // table does not contain class ID, so we must remove previous entries.
                scGeomWriter->Delete(GetContainingDbObjectName(), GetColumnName());

                // Now add the association record.
                scGeomWriter->Add();
            }
            break;
        }
	case FdoSchemaElementState_Modified:
            pWriter->SetIsReadOnly( GetReadOnly() );
            pWriter->SetDescription( GetDescription() );
            pWriter->SetDataType( FdoStringP::Format(L"%d", GetGeometryTypes()) );
			pWriter->SetHasElevation( GetHasElevation() );
			pWriter->SetHasMeasure( GetHasMeasure() );
            pWriter->Modify( pTopClass->GetId(), GetName() );
            if (mAssociatedScId >= 0) {
                FdoSmPhSpatialContextGeomWriterP scGeomWriter = pPhysical->GetSpatialContextGeomWriter();
                scGeomWriter->SetScId(mAssociatedScId);
                scGeomWriter->SetGeomTableName( GetContainingDbObjectName() );
                scGeomWriter->SetGeomColumnName( GetColumnName() );
                FdoInt32 dimensionality = FdoDimensionality_XY;
                if (GetHasElevation())
                    dimensionality |= FdoDimensionality_Z;
                if (GetHasMeasure())
                    dimensionality |= FdoDimensionality_M;
                scGeomWriter->SetDimensionality( dimensionality );
                scGeomWriter->Modify( GetContainingDbObjectName(), GetColumnName() );
            }
		    break;
	}
}

FdoSchemaExceptionP FdoSmLpGeometricPropertyDefinition::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FdoSmLpSimplePropertyDefinition::Errors2Exception(pFirstException);

	return pException;
}

void FdoSmLpGeometricPropertyDefinition::XMLSerialize( FILE* xmlFp, int ref ) const
{
	if ( ref == 0 ) {
		fprintf( xmlFp, "<property xsi:type=\"%ls\" name=\"%ls\" description=\"%ls\"\n geometryTypes=\"%ld\" hasElevation=\"%s\" hasMeasure=\"%s\"\n tableName=\"%ls\" columnName=\"%ls\" colCreator=\"%s\" fixedCol=\"%s\" >\n",
                (FdoString*) FdoSmLpPropertyTypeMapper::Type2String(GetPropertyType()),
				GetName(), GetDescription(),
				GetGeometryTypes(),
				GetHasElevation() ? "True" : "False",
				GetHasMeasure() ? "True" : "False",
				GetContainingDbObjectName(),
				GetColumnName(),
				GetIsColumnCreator() ? "True" : "False",
				GetIsFixedColumn() ? "True" : "False"
		);

		if ( RefDefiningClass() &&
			RefBaseProperty() ) 
			fprintf( xmlFp, "<Inherited baseClass=\"%ls\" />\n",
				RefDefiningClass()->GetName()
			);

		if ( RefColumn() ) 
			RefColumn()->XMLSerialize(xmlFp, ref);

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );

		fprintf( xmlFp, "</property>\n" );
    }
	else {
		fprintf( xmlFp, "<property xsi:type=\"%ls\" name=\"%ls\" />\n",
				(FdoString*) FdoSmLpPropertyTypeMapper::Type2String(GetPropertyType()),
				GetName()
		);
	}
}

void FdoSmLpGeometricPropertyDefinition::Finalize()
{
	if ( GetState() != FdoSmObjectState_Final ) {
        FdoSmLpSimplePropertyDefinition::Finalize();

        FdoSmOvGeometricColumnType columnType = GetGeometricColumnType();
        bool hasZColumn = false;
        if (FdoSmOvGeometricColumnType_Double == columnType &&
            mColumnNameZ != NULL && mColumnNameZ.GetLength() > 0) {
                hasZColumn = true;
        }

	    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	    FdoSmLpClassDefinition* pContainingClass = 
            dynamic_cast<FdoSmLpClassDefinition*>((FdoSmLpSchemaElement*) (GetParent()));
        
        FdoSmLpGeometricPropertyP pPrevProperty = 
            GetPrevProperty().p->SmartCast<FdoSmLpGeometricPropertyDefinition>();

        // Get the current column, table and table name settings 

        FdoSmPhDbObjectP pPhDbObject = GetContainingDbObject();
        
        FdoStringP pDbObjectName = pPhDbObject ? pPhDbObject->GetName() : L"";
        
        // Geometry column is kept in the target class table.
        // Copied columns are also kept in the target class table.
        FdoStringP dbObjectName = pContainingClass->GetDbObjectName();
        pPhDbObject = pPhysical->FindDbObject(dbObjectName);

        // Containing table can now be set.
	    SetContainingDbObject(pPhDbObject, pDbObjectName);

        // Determine column depending on whether this is a new property.
        if ( (GetElementState() == FdoSchemaElementState_Added) || 
            (GetIsFromFdo() && (FdoSmOvGeometricColumnType_Double != columnType))
        ) {

            if ( pPrevProperty && (FdoStringP(this->GetContainingDbObjectName()).ICompare(pPrevProperty->GetContainingDbObjectName()) == 0) ) {
	    	    // New property in same table as the base property. New property
	    	    // just latches onto base property column.
                if (FdoSmOvGeometricColumnType_Double == columnType) {
    	    	    SetColumnX( pPrevProperty->GetColumnX() );
                    SetIsColumnCreatorX(true);
    	    	    SetColumnY( pPrevProperty->GetColumnY() );
                    SetIsColumnCreatorY(true);
                    if (hasZColumn) {
        	    	    SetColumnZ( pPrevProperty->GetColumnZ() );
                        SetIsColumnCreatorZ(true);
                    }
                } else {
    	    	    SetColumn( pPrevProperty->GetColumn() );
                }
            } else {
		        // Tables are different. Must create new column for this property.
                if (FdoSmOvGeometricColumnType_Double == columnType) {
                    SetColumnX( NewOrdColumn(pPhDbObject, GetColumnNameX(), true) );
                    SetIsColumnCreatorX(true);
                    SetColumnY( NewOrdColumn(pPhDbObject, GetColumnNameY(), true) );
                    SetIsColumnCreatorY(true);
                    FdoString * columnNameZ = GetColumnNameZ();
                    if (hasZColumn) {
                        SetColumnZ( NewOrdColumn(pPhDbObject, columnNameZ, true) );
                        SetIsColumnCreatorZ(true);
                    }
                } else {
                    CreateColumn(pPhDbObject);
                }
            }

#pragma message ("ToDo: Support inherited SI columns and persisted SI column names")
            if (mWantSiColumns)
                AddSiColumns();
        }
        else if ( GetElementState() == FdoSchemaElementState_Modified ) {
	        if ( pPhDbObject ) {
                if (FdoSmOvGeometricColumnType_Double == columnType) {
    		        SetColumnX( pPhDbObject->GetColumns()->FindItem(GetColumnNameX()) );
    		        SetColumnY( pPhDbObject->GetColumns()->FindItem(GetColumnNameY()) );
                    if (hasZColumn)
    		            SetColumnZ( pPhDbObject->GetColumns()->FindItem(GetColumnNameZ()) );
                } else {
    		        SetColumn( pPhDbObject->GetColumns()->FindItem(GetColumnName()) );
                }
                if (mWantSiColumns)
                    AddSiColumns();
	        }
            if ( GetColumn() ) {

                GetColumn()->SetElementState( GetElementState() );
                FdoSmPhScInfoP scInfo = CreateSpatialContextInfo();
                FdoSmPhColumnP pPhColumn = GetColumn();
                FdoSmPhColumnGeomP pPhColumnGeom = pPhColumn->SmartCast<FdoSmPhColumnGeom>();
                if (pPhColumnGeom != NULL)
                    pPhColumnGeom->SetSpatialContextInfo( scInfo );
            }
        }
        else {
            // For existing property, find the column in the containing table.
	        if ( pPhDbObject ) {
                if (FdoSmOvGeometricColumnType_Double == columnType) {
    		        SetColumnX( pPhDbObject->GetColumns()->FindItem(GetColumnNameX()) );
    		        SetColumnY( pPhDbObject->GetColumns()->FindItem(GetColumnNameY()) );
                    if (hasZColumn)
        		        SetColumnZ( pPhDbObject->GetColumns()->FindItem(GetColumnNameZ()) );
                } else {
    		        SetColumn( pPhDbObject->GetColumns()->FindItem(GetColumnName()) );
                }

                if (mWantSiColumns && TableHasSpatialIndexColumns())
                    AddSiColumns();
	        }

            // Delete columns that this property created.
            if (GetElementState() == FdoSchemaElementState_Deleted) {
	            FdoStringP dbObjectName = GetContainingDbObjectName();
	            FdoStringP baseDbObjectName;

	            if ( GetBaseProperty() ) 
		            baseDbObjectName = GetPrevProperty()->GetContainingDbObjectName();

	            // Keep the columns if they are used by the base property. 
                // TODO: create an error when used by base property. A property
                // should never be the creator of the column for its base 
                // property.
                if ( dbObjectName.ICompare(baseDbObjectName) != 0 ) {

                    // Set the columns for deletion.
                    if ( GetColumn() && GetIsColumnCreator())
		                ((FdoSmPhColumn*) GetColumn())->SetElementState( GetElementState() );
                    if ( GetColumnX() && GetIsColumnCreatorX())
		                ((FdoSmPhColumn*) GetColumnX())->SetElementState( GetElementState() );
                    if ( GetColumnY() && GetIsColumnCreatorY())
		                ((FdoSmPhColumn*) GetColumnY())->SetElementState( GetElementState() );
                    if ( GetColumnZ() && GetIsColumnCreatorZ())
		                ((FdoSmPhColumn*) GetColumnZ())->SetElementState( GetElementState() );
                    if ( GetColumnSi1() && GetIsColumnCreatorSi1()) {
		                ((FdoSmPhColumn*) GetColumnSi1())->SetElementState( GetElementState() );
                        FdoSmPhTableP table = pPhDbObject->SmartCast<FdoSmPhTable>();
                        if (table != NULL) {
                            FdoSmPhIndexesP indexes = table->GetIndexes();
                            if (indexes != NULL) {
                                FdoStringP siName = GetColumnNameSi1();
                                siName += FDOSMLP_SI_COLUMN_IDX_SUFFIX;
                                FdoSmPhIndexP index = indexes->FindItem(siName);
                                if (index != NULL)
                                    index->SetElementState(GetElementState());
                            }
                        }
                    }
                    if ( GetColumnSi2() && GetIsColumnCreatorSi2()) {
		                ((FdoSmPhColumn*) GetColumnSi2())->SetElementState( GetElementState() );
                        FdoSmPhTableP table = pPhDbObject->SmartCast<FdoSmPhTable>();
                        if (table != NULL) {
                            FdoSmPhIndexesP indexes = table->GetIndexes();
                            if (indexes != NULL) {
                                FdoStringP siName = GetColumnNameSi2();
                                siName += FDOSMLP_SI_COLUMN_IDX_SUFFIX;
                                FdoSmPhIndexP index = indexes->FindItem(siName);
                                if (index != NULL)
                                    index->SetElementState(GetElementState());
                            }
                        }
                    }
                }
            }
        }
    }
}

FdoSmPhScInfoP FdoSmLpGeometricPropertyDefinition::CreateSpatialContextInfo()
{
    FdoSmLpSchemasP schemas = GetLogicalPhysicalSchema()->GetSchemas();
    FdoSmLpSpatialContextsP scs = schemas->GetSpatialContexts();

    FdoSmLpSpatialContextP sc = scs->FindItem(mAssociatedSCName);
    if (sc == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoSmPhScInfoP scInfo = FdoSmPhScInfo::Create();
    scInfo->mSrid = sc->GetSrid();
    scInfo->mCoordSysName = sc->GetCoordinateSystem();
    scInfo->mExtent = sc->GetExtent();
    scInfo->mXYTolerance = sc->GetXYTolerance();
    scInfo->mZTolerance = sc->GetZTolerance();

    return scInfo;
}

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::NewColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName )
{
    FdoSmPhScInfoP scInfo = CreateSpatialContextInfo();

    return dbObject->CreateColumnGeom(
        columnName,
        scInfo,
        true,
        GetHasElevation(),
        GetHasMeasure(),
        GetRootColumnName()
    );

}

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::NewSiColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName, bool isNullable )
{
    FdoSmPhColumnP column = dbObject->CreateColumnChar(
        columnName,
        isNullable,
        FDOSMLP_SI_COLUMN_LENGTH,
        columnName
    );

    FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();
    if (table != NULL) {
        FdoSmPhIndexP idx = table->CreateIndex(columnName + FDOSMLP_SI_COLUMN_IDX_SUFFIX, false);
        idx->AddColumn(column);
    }
    return column;
}

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::NewOrdColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName, bool isNullable )
{
	return dbObject->CreateColumnDouble(
        columnName,
        isNullable,
        columnName
    );
}

void FdoSmLpGeometricPropertyDefinition::AddSiColumns()
{
    // We actually use the property name as the basis for the
    // spatial index column names.  There isn't always a single
    // geometric column on which to base the SI columns.
    FdoString * columnName = GetName();

    // Defend against the system-defined Bounds column.  We do not really
    // support it, and if we did, it would never need spatial index columns.
    if ( wcscmp(columnName, L"Bounds") == 0 )
        return;

    // Make sure that the SI columns are not already set.
    if (NULL != mColumnSi1.p || NULL != mColumnSi2.p)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_4_UNREADY)));

	FdoSmPhMgrP pPhysical	        = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoSmPhDbObjectP pPhDbObject    = pPhysical->FindDbObject( GetContainingDbObjectName() );

    if (NULL != pPhDbObject.p)
    {
        FdoSmPhColumnP column;
        FdoSmPhColumnsP columns = pPhDbObject->GetColumns();

        FdoStringP name = columnName;
        name += FDOSMLP_SI_COLUMN_1_NAME;
        name = name.Upper();
        // TODO: After column name lookup from F_SPATIALCONTEXTGEOM is supported, this
        // code should call UniqueColumnName instead of CensorDbObjectName.  We call
        // CensorDbObjectName temporarily here, because this code is used to formulate
        // names of existing SI columns, in addition to the names of new ones.
        // After lookup is supported, this should only be used for new SI columns.
        name = pPhysical->CensorDbObjectName(name);
    	//name = ((FdoSmLpClassDefinition*)RefParentClass())->UniqueColumnName( 
        //    pPhDbObject, this, name, false);
        if (NULL != columns)
            column = columns->FindItem(name);
        if (NULL == column.p)
            column = NewSiColumn(pPhDbObject, name, true);
        SetColumnSi1(column);

        column = (FdoSmPhColumn*) NULL;
        name = columnName;
        name += FDOSMLP_SI_COLUMN_2_NAME;
        name = name.Upper();
        name = pPhysical->CensorDbObjectName(name);
    	//name = ((FdoSmLpClassDefinition*)RefParentClass())->UniqueColumnName( 
        //    pPhDbObject, this, name, false);
        if (NULL != columns)
            column = columns->FindItem(name);
        if (NULL == column.p)
            column = NewSiColumn(pPhDbObject, name, true);
        SetColumnSi2(column);
    }
}


//////////////////////////////////////////////////////////////////////

// Replicate some column-oriented accessor and mutator methods
// inherited from FdoSmLpSimplePropertyDefinition, but for the
// geometric property's possible extra columns.

//////////////////////////////////////////////////////////////////////
// Si1 column

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::GetColumnSi1()
{
    Finalize();

    return mColumnSi1;
}

const FdoSmPhColumn* FdoSmLpGeometricPropertyDefinition::RefColumnSi1() const
{
    return (FdoSmPhColumn*) ((FdoSmLpGeometricPropertyDefinition*) this)->GetColumnSi1();
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetColumnNameSi1() const
{
    ((FdoSmLpGeometricPropertyDefinition*) this)->Finalize();

    return mColumnNameSi1;
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetRootColumnNameSi1() const
{
    return mRootColumnNameSi1;
}

void FdoSmLpGeometricPropertyDefinition::SetColumnSi1( FdoSmPhColumnP column )
{
    mColumnSi1 = column;
    if ( column ) {
        SetColumnNameSi1( column->GetName() );
        // When column in view on foreign table, tell it what its root
        // column name is.
        if ( mRootColumnNameSi1.GetLength() > 0 ) 
            mColumnSi1->SetRootName( mRootColumnNameSi1 );
    }
}

void FdoSmLpGeometricPropertyDefinition::SetColumnNameSi1( FdoString* columnName )
{
    mColumnNameSi1 = columnName;
}

void FdoSmLpGeometricPropertyDefinition::SetRootColumnNameSi1( FdoString* columnName )
{
    mRootColumnNameSi1 = columnName;
}

//////////////////////////////////////////////////////////////////////
// Si2 column

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::GetColumnSi2()
{
    Finalize();

    return mColumnSi2;
}

const FdoSmPhColumn* FdoSmLpGeometricPropertyDefinition::RefColumnSi2() const
{
    return (FdoSmPhColumn*) ((FdoSmLpGeometricPropertyDefinition*) this)->GetColumnSi2();
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetColumnNameSi2() const
{
    ((FdoSmLpGeometricPropertyDefinition*) this)->Finalize();

    return mColumnNameSi2;
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetRootColumnNameSi2() const
{
    return mRootColumnNameSi2;
}

void FdoSmLpGeometricPropertyDefinition::SetColumnSi2( FdoSmPhColumnP column )
{
    mColumnSi2 = column;
    if ( column ) {
        SetColumnNameSi2( column->GetName() );
        // When column in view on foreign table, tell it what its root
        // column name is.
        if ( mRootColumnNameSi2.GetLength() > 0 ) 
            mColumnSi2->SetRootName( mRootColumnNameSi2 );
    }
}

void FdoSmLpGeometricPropertyDefinition::SetColumnNameSi2( FdoString* columnName )
{
    mColumnNameSi2 = columnName;
}

void FdoSmLpGeometricPropertyDefinition::SetRootColumnNameSi2( FdoString* columnName )
{
    mRootColumnNameSi2 = columnName;
}

//////////////////////////////////////////////////////////////////////
// X column

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::GetColumnX()
{
    Finalize();

    return mColumnX;
}

const FdoSmPhColumn* FdoSmLpGeometricPropertyDefinition::RefColumnX() const
{
    return (FdoSmPhColumn*) ((FdoSmLpGeometricPropertyDefinition*) this)->GetColumnX();
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetColumnNameX() const
{
    ((FdoSmLpGeometricPropertyDefinition*) this)->Finalize();

    return mColumnNameX;
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetRootColumnNameX() const
{
    return mRootColumnNameX;
}

void FdoSmLpGeometricPropertyDefinition::SetColumnX( FdoSmPhColumnP column )
{
    mColumnX = column;
    if ( column ) {
        SetColumnNameX( column->GetName() );
        // When column in view on foreign table, tell it what its root
        // column name is.
        if ( mRootColumnNameX.GetLength() > 0 ) 
            mColumnX->SetRootName( mRootColumnNameX );
    }
}

void FdoSmLpGeometricPropertyDefinition::SetColumnNameX( FdoString* columnName )
{
    mColumnNameX = columnName;
}

void FdoSmLpGeometricPropertyDefinition::SetRootColumnNameX( FdoString* columnName )
{
    mRootColumnNameX = columnName;
}

//////////////////////////////////////////////////////////////////////
// Y column

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::GetColumnY()
{
    Finalize();

    return mColumnY;
}

const FdoSmPhColumn* FdoSmLpGeometricPropertyDefinition::RefColumnY() const
{
    return (FdoSmPhColumn*) ((FdoSmLpGeometricPropertyDefinition*) this)->GetColumnY();
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetColumnNameY() const
{
    ((FdoSmLpGeometricPropertyDefinition*) this)->Finalize();

    return mColumnNameY;
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetRootColumnNameY() const
{
    return mRootColumnNameY;
}

void FdoSmLpGeometricPropertyDefinition::SetColumnY( FdoSmPhColumnP column )
{
    mColumnY = column;
    if ( column ) {
        SetColumnNameY( column->GetName() );
        // When column in view on foreign table, tell it what its root
        // column name is.
        if ( mRootColumnNameY.GetLength() > 0 ) 
            mColumnY->SetRootName( mRootColumnNameY );
    }
}

void FdoSmLpGeometricPropertyDefinition::SetColumnNameY( FdoString* columnName )
{
    mColumnNameY = columnName;
}

void FdoSmLpGeometricPropertyDefinition::SetRootColumnNameY( FdoString* columnName )
{
    mRootColumnNameY = columnName;
}

//////////////////////////////////////////////////////////////////////
// Z column

FdoSmPhColumnP FdoSmLpGeometricPropertyDefinition::GetColumnZ()
{
    Finalize();

    return mColumnZ;
}

const FdoSmPhColumn* FdoSmLpGeometricPropertyDefinition::RefColumnZ() const
{
    return (FdoSmPhColumn*) ((FdoSmLpGeometricPropertyDefinition*) this)->GetColumnZ();
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetColumnNameZ() const
{
    ((FdoSmLpGeometricPropertyDefinition*) this)->Finalize();

    return mColumnNameZ;
}

FdoString* FdoSmLpGeometricPropertyDefinition::GetRootColumnNameZ() const
{
    return mRootColumnNameZ;
}

void FdoSmLpGeometricPropertyDefinition::SetColumnZ( FdoSmPhColumnP column )
{
    mColumnZ = column;
    if ( column ) {
        SetColumnNameZ( column->GetName() );
        // When column in view on foreign table, tell it what its root
        // column name is.
        if ( mRootColumnNameZ.GetLength() > 0 ) 
            mColumnZ->SetRootName( mRootColumnNameZ );
    }
}

void FdoSmLpGeometricPropertyDefinition::SetColumnNameZ( FdoString* columnName )
{
    mColumnNameZ = columnName;
}

void FdoSmLpGeometricPropertyDefinition::SetRootColumnNameZ( FdoString* columnName )
{
    mRootColumnNameZ = columnName;
}


//////////////////////////////////////////////////////////////////////

bool FdoSmLpGeometricPropertyDefinition::TableHasSpatialIndexColumns()
{
    bool hasSiColumns = false;

    // Both columns must be here.

    FdoString * columnName = GetName();
    if ( wcscmp(columnName, L"Bounds") == 0 )
        return hasSiColumns;

	FdoSmPhMgrP pPhysical	        = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoSmPhDbObjectP pPhDbObject    = pPhysical->FindDbObject( GetContainingDbObjectName() );

    if (NULL != pPhDbObject.p)
    {
        FdoSmPhColumnP column;
        const FdoSmPhColumnCollection * columns = pPhDbObject->RefColumns();

        if (NULL != columns)
        {
            FdoStringP name1 = columnName;
            name1 += FDOSMLP_SI_COLUMN_1_NAME;
            FdoStringP name2 = columnName;
            name2 += FDOSMLP_SI_COLUMN_1_NAME;
            if (NULL != columns->RefItem(name1) && NULL != columns->RefItem(name2))
                hasSiColumns = true;
        }
    }

    return hasSiColumns;
}

void FdoSmLpGeometricPropertyDefinition::AddGeometryTypeChangeError( FdoInt32 newTypes, bool bEmptyGeom ) 
{
    if ( bEmptyGeom )
	    GetErrors()->Add( FdoSmErrorType_Other, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
			        FDO_NLSID(FDOSM_164),
			        (FdoString*) GetQName(), 
			        mGeometryTypes,
			        newTypes
                )
            )
	    );
    else
        GetErrors()->Add( FdoSmErrorType_Other, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
			        FDO_NLSID(FDOSM_391),
			        (FdoString*) GetQName(), 
			        mGeometryTypes,
			        newTypes
		        )
            )
	    );
}

void FdoSmLpGeometricPropertyDefinition::AddHasElevationChangeError() 
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_319),
			    (FdoString*) GetQName() 
		    )
        )
	);
}

void FdoSmLpGeometricPropertyDefinition::AddDimensionalityError() 
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_329),
                GetName(), 
                GetParent()->GetName()
            )
        )
    );
}

void FdoSmLpGeometricPropertyDefinition::AddAssociatedSCChangeError() 
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_369),
                GetName() 
            )
        )
    );
}

void FdoSmLpGeometricPropertyDefinition::AddSCNotFoundError()
{
	GetErrors()->Add( FdoSmErrorType_SpatialMismatch, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_372),
                GetName(),
                RefDefiningClass()->GetName()
            )
        )
    );
}

void FdoSmLpGeometricPropertyDefinition::AddSCTableNotFoundError()
{
	GetErrors()->Add( FdoSmErrorType_SpatialMismatch, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_374),
                GetName(),
                RefDefiningClass()->GetName()
            )
        )
    );
}

void FdoSmLpGeometricPropertyDefinition::AddOverrideConflictError() 
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_409),
			    (FdoString*) GetQName() 
            )
		)
	);
}

/*
int FdoSmLpGeometricPropertyDefinition::GetDimensionality()
{
    int  dimensionality;

    if (      GetHasElevation() && GetHasMeasure())
        dimensionality = DBI_XYZM;
    else if ( GetHasElevation() && !GetHasMeasure())
        dimensionality = DBI_XYZ;
    else if ( !GetHasElevation() && GetHasMeasure())
        dimensionality = DBI_XYM;
    else
        dimensionality = DBI_XY;

    return dimensionality;
}

FdoStringP FdoSmLpGeometricPropertyDefinition::GetActiveSpatialContextName()
{
   	FdoSmPhysicalSchema*	pPhysical = Get_LogicalPhysicalSchema()->Get_PhysicalSchema();
	DbiConnection*          pConn = pPhysical->GetConnection();
    dbi_plan_info_def       plan_info;
    int                     found;

    dbi_pn_id_t plan_num = ((DbiConnection*) pConn)->dbi_plan_active_get();
    ((DbiConnection*) pConn)->dbi_plan_get( plan_num, &plan_info, &found );
            
    return FdoStringP( plan_info.name );
}
*/
bool FdoSmLpGeometricPropertyDefinition::CheckSupportedGeometricTypes( FdoGeometricPropertyDefinition* pFdoGeomProp )
{
    int     geometryTypes = pFdoGeomProp->GetGeometryTypes();
    bool    err = false;

    // Can update if the new settings cover the old settings. Error otherwise.
    err =   (((mGeometryTypes & FdoGeometricType_Point)   != 0) && ((geometryTypes & FdoGeometricType_Point) == 0 )) ||
            (((mGeometryTypes & FdoGeometricType_Curve)   != 0) && ((geometryTypes & FdoGeometricType_Curve) == 0 )) ||
            (((mGeometryTypes & FdoGeometricType_Surface) != 0) && ((geometryTypes & FdoGeometricType_Surface) == 0 )) ||
            (((mGeometryTypes & FdoGeometricType_Solid)   != 0) && ((geometryTypes & FdoGeometricType_Solid) == 0 ));

    // If error, it's still OK provided the geometry column contains no data.
    // NOTE: A less strict check would be to count the instances of no longer allowed shape types,
    // but this was too complicated in now-obsolete schemas.
    if ( err && GetColumn()->GetHasValues() )
    {
        AddGeometryTypeChangeError( pFdoGeomProp->GetGeometryTypes(), false );
    } 
    else
    {
        err = false;
    }

    return !err;
}

