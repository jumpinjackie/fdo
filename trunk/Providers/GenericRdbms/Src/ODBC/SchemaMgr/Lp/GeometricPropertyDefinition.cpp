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
#include "GeometricPropertyDefinition.h"
#include <Sm/Lp/FeatureClass.h>

FdoSmLpOdbcGeometricPropertyDefinition::FdoSmLpOdbcGeometricPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdGeometricPropertyDefinition(propReader, parent)
{
    SetGeometricColumnType(FdoSmOvGeometricColumnType_Double);
    SetGeometricContentType(FdoSmOvGeometricContentType_Ordinates);
}

FdoSmLpOdbcGeometricPropertyDefinition::FdoSmLpOdbcGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdGeometricPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
    SetGeometricColumnType(FdoSmOvGeometricColumnType_Double);
    SetGeometricContentType(FdoSmOvGeometricContentType_Ordinates);
    SetColumnName(NULL);
    SetColumn((FdoSmPhColumn *)NULL);
    SetColumnNameX(L"X");
    SetColumnNameY(L"Y");
    if (this->GetHasElevation())
        SetColumnNameZ(L"Z");
    else
        SetColumnNameZ(NULL);
}

FdoSmLpOdbcGeometricPropertyDefinition::FdoSmLpOdbcGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* pFdoProp, 
    FdoString * columnNameX,
    FdoString * columnNameY,
    FdoString * columnNameZ,
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdGeometricPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
    SetGeometricColumnType(FdoSmOvGeometricColumnType_Double);
    SetGeometricContentType(FdoSmOvGeometricContentType_Ordinates);
    SetColumnName(NULL);
    SetColumn((FdoSmPhColumn *)NULL);
    SetColumnNameX(columnNameX);
    SetColumnNameY(columnNameY);
    SetColumnNameZ(columnNameZ);
}

FdoSmLpOdbcGeometricPropertyDefinition::FdoSmLpOdbcGeometricPropertyDefinition(
    FdoSmLpGeometricPropertyP pBaseProperty,
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
    FdoSmLpGrdGeometricPropertyDefinition(
        pBaseProperty,
        pTargetClass,
        logicalName,
        physicalName,
        bInherit,
        pPropOverrides
    )
{
    SetGeometricColumnType(FdoSmOvGeometricColumnType_Double);
    SetGeometricContentType(FdoSmOvGeometricContentType_Ordinates);
    SetColumnName(NULL);
    SetColumn((FdoSmPhColumn *)NULL);
    SetColumnNameX(L"X");
    SetColumnNameY(L"Y");
    if (this->GetHasElevation())
        SetColumnNameZ(L"Z");
    else
        SetColumnNameZ(NULL);
}

bool FdoSmLpOdbcGeometricPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoOdbcOvClassDefinition* odbcClassMapping = (FdoOdbcOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;  

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping = GetSchemaMappings( bIncludeDefaults );
    FdoOdbcOvPropertyDefinition* odbcPropMapping = 
        dynamic_cast<FdoOdbcOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if ( odbcPropMapping ) {
        FdoOdbcOvPropertiesP(odbcClassMapping->GetProperties())->Add( odbcPropMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpOdbcGeometricPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoOdbcOvGeometricPropertyDefinition* propMapping = FdoOdbcOvGeometricPropertyDefinition::Create( GetName() );
    FdoPhysicalPropertyMappingP retPropMapping = (FdoPhysicalPropertyMapping*) propMapping;  
    bool bHasMappings = false;

    ((FdoSmLpOdbcGeometricPropertyDefinition*)this)->Finalize();

    // This code could really be at the FdoRdbms level, removing the need for
    // implementation in each FdoRdbms-based provider.  However, at least here we
    // can discriminate between what is the default for each provider.  This is
    // why we do not write out the geometric column type or content type -- only
    // ordinate columns are supported by the ODBC Provider, and overrides are
    // provider-specific, so there is no non-default needed when writing out
    // an override file.  We'll just write out the ordinate column name overrides.

    FdoStringP columnNameX = this->GetColumnNameX();
    FdoStringP columnNameY = this->GetColumnNameY();
    FdoStringP columnNameZ = this->GetColumnNameZ();

    if (columnNameX.GetLength() > 0 && columnNameX != L"X") {
        propMapping->SetXColumnName(columnNameX);
        bHasMappings = true;
    }

    if (columnNameY.GetLength() > 0 && columnNameY != L"Y") {
        propMapping->SetYColumnName(columnNameY);
        bHasMappings = true;
    }

    if (columnNameZ.GetLength() > 0 && columnNameZ != L"Z") {
        propMapping->SetZColumnName(columnNameZ);
        bHasMappings = true;
    }

    // return NULL if no mappings.
    if ( !bHasMappings ) 
        retPropMapping = NULL;

    return retPropMapping;
}

void FdoSmLpOdbcGeometricPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,       
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Do updates from generic physical mappings:
    FdoSmLpGrdGeometricPropertyDefinition::Update(pFdoProp, elementState, pPropOverrides, bIgnoreStates);

    //FdoOdbcOvGeometricPropertyDefinition* odbcGeomPropOverrides = (FdoOdbcOvGeometricPropertyDefinition*)(FdoRdbmsOvGeometricPropertyDefinition*)(FdoRdbmsOvPropertyDefinition*)pPropOverrides;
    FdoOdbcOvGeometricPropertyDefinition* odbcGeomPropOverrides = dynamic_cast<FdoOdbcOvGeometricPropertyDefinition*>(pPropOverrides);
    if (odbcGeomPropOverrides)
    {
        FdoRdbmsOvGeometricColumn* odbcGeomColOverrides = odbcGeomPropOverrides->GetColumn();
        if (odbcGeomColOverrides)
        {
            if ( (elementState == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
                // no ODBC-specific mappings to update, currently.
            }
            else
            {
                // TODO: log an exception via GetErrors()->Add()  ?
            }
        }
    }
}

FdoSmLpPropertyP FdoSmLpOdbcGeometricPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpOdbcGeometricPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpGeometricPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpOdbcGeometricPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpOdbcGeometricPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpGeometricPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}
