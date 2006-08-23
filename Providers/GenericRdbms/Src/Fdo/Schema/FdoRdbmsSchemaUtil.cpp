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

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsException.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Sm/Lp/PropertyMappingConcrete.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include "FdoCommonOSUtil.h"

wchar_t* FdoRdbmsSchemaUtil::makePropName( const wchar_t* scope, const wchar_t* name )
{
    size_t  size = wcslen(scope)+wcslen(name)+2;
    if( mBuffer == NULL )
    {
        mBufferSize = size;
        mBuffer = new wchar_t[mBufferSize];
        if( mBuffer == NULL )
            throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));
    }
    else if( mBufferSize < size )
    {
        delete[] mBuffer;
        mBufferSize = size;
        mBuffer = new wchar_t[mBufferSize];
        if( mBuffer == NULL )
            throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));
    }
    if( wcslen( scope ) != 0 )
    {
        wcscpy( mBuffer, scope );
        wcscat( mBuffer, L"." );
        wcscat( mBuffer, name );
    }
    else
    {
        wcscpy( mBuffer, name );
    }
    return mBuffer;
}

FdoRdbmsSchemaUtil::FdoRdbmsSchemaUtil()
{
    mBuffer = NULL;
    mBufferSize = 0;
	mSkipCheck = false;
}

FdoRdbmsSchemaUtil::FdoRdbmsSchemaUtil(DbiConnection *connection)
{
    mUtility = connection->GetUtility();
    mBuffer = NULL;
    mBufferSize = 0;
	mSkipCheck = false;
}

FdoRdbmsSchemaUtil::~FdoRdbmsSchemaUtil()
{
    if (mBuffer)
        delete[] mBuffer;
}

const char *FdoRdbmsSchemaUtil::MakeDBValidName(const wchar_t *name)
{
    return mUtility->UnicodeToUtf8(name);
//TODO:    return mUtility->UnicodeToUtf8( mSchemaMgr->RefPhysicalSchema()->CensorDbObjName( name ) );
}

const FdoSmLpClassDefinition* FdoRdbmsSchemaUtil::GetClass(const wchar_t* className)
{
    const FdoSmLpClassDefinition* currentClass = NULL;
    const FdoSmLpPropertyDefinition *propertyDefinition = NULL;
    const FdoSmLpObjectPropertyDefinition* objProp = NULL;

    // className can be class, class.objectproperty or prepended with the schema name as schema:class ...
    FdoPtr<FdoIdentifier>classIdent = FdoIdentifier::Create(className);

    FdoStringP schemaName = classIdent->GetSchemaName();

    const FdoSmLpSchemaCollection* logPhysSchemas = mSchemaMgr->RefLogicalPhysicalSchemas();
    int   count;
    const wchar_t **scopes = classIdent->GetScope( count );
    if ( count == 0 )
        currentClass = logPhysSchemas->FindClass(schemaName, classIdent->GetName());
    else
        currentClass = logPhysSchemas->FindClass(schemaName, scopes[0]);

    for (int j=1; j<=count && currentClass; j++)
    {
            const wchar_t* objPropName;
            if( j == count )
                objPropName = classIdent->GetName();
            else
                objPropName = scopes[j];
            propertyDefinition = currentClass->RefProperties()->RefItem( objPropName );
            if (!propertyDefinition)
                throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_231, "Object property '%1$ls' not found", objPropName));
            if( propertyDefinition->GetPropertyType() != FdoPropertyType_ObjectProperty )
                throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_24, "Expected object property"));
            objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
            currentClass = objProp->RefTargetClass();

    }

    return currentClass;
}

FdoPropertyValueCollection* FdoRdbmsSchemaUtil::GetPropertyValues( const wchar_t* className, bool bGetAutoGen )
{
    const FdoSmLpClassDefinition* currentClass = NULL;
    FdoPropertyValueCollection* propertyValues = FdoPropertyValueCollection::Create();

    const FdoSmLpPropertyDefinition *propertyDefinition = NULL;
//  const FdoSmLpObjectPropertyDefinition* objProp = NULL;
    bool  bAddPrimaryKeys = false;

    FdoStringsP strings = FdoStringCollection::Create(className, L".");

    // If className is class.objectproperty, we want to add the primary keys.
    if (strings->GetCount() > 1)
        bAddPrimaryKeys = true;

    currentClass = GetClass(className);

    AppendProperties(L"", currentClass, bAddPrimaryKeys, bGetAutoGen, propertyValues );

    return propertyValues;
}

void FdoRdbmsSchemaUtil::AppendProperties( const wchar_t* scope, const FdoSmLpClassDefinition* currentClass, bool bAddPrimaryKeys, bool bGetAutoGen, FdoPropertyValueCollection* propertyValues )
{
    const FdoSmLpDataPropertyDefinition *dataPropertyDefinition = NULL;
    const FdoSmLpObjectPropertyDefinition *objectPropertyDefinition = NULL;
    const FdoSmLpGeometricPropertyDefinition *geometryPropertyDefinition = NULL;
    const FdoSmLpAssociationPropertyDefinition* associationPropertyDefinition  = NULL;
    FdoPropertyValue *propValue = NULL;
    FdoGeometryValue *geomValue = NULL;
    bool              bGetProperty = false;

    if( currentClass == NULL )
        return;

    const FdoSmLpPropertyDefinitionCollection *properties = currentClass->RefProperties();
    for (int i=0; i<properties->GetCount(); i++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = properties->RefItem(i);
        if(  propertyDefinition  == NULL )
            continue;

        switch (propertyDefinition->GetPropertyType() )
        {
            case FdoPropertyType_DataProperty:
                dataPropertyDefinition = static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);

                if ( bGetAutoGen )
                {
                    // Get all non-system autogenerated properties
                    // (except FeatId whose value is set specially
                    //  in FdoRdbmsInsertCommand).
                    bGetProperty =
                        dataPropertyDefinition->GetIsSystem() == false &&
                        dataPropertyDefinition->GetIsFeatId() == false &&
                        dataPropertyDefinition->GetIsAutoGenerated() == true;
                }
                else
                {
                    // Get all client-settable properties
                    // (FeatId properties and non-system non-readonly properties;
                    //  FeatId is not client-settable but is handled specially)
                    bGetProperty =
                        (dataPropertyDefinition->GetIsSystem() == false &&
                        dataPropertyDefinition->GetReadOnly() == false &&
                        dataPropertyDefinition->GetIsAutoGenerated() == false) ||
                        dataPropertyDefinition->GetIsFeatId() == true;
                }

                if ( bGetProperty )
                {
                    const FdoSmLpDbObject* currentTable = currentClass->RefDbObject();
                    if ( currentTable )
                    {
                        const FdoSmPhColumnCollection* fkCols = currentTable->RefSourceColumns();
                        const wchar_t *name = dataPropertyDefinition->GetName();
                        if (bAddPrimaryKeys || !fkCols->RefItem(name))
                        {
                            propValue = FdoPropertyValue::Create();
                            propValue->SetName( makePropName( scope, propertyDefinition->GetName() ) );
                            propertyValues->Add(propValue);
                            propValue->Release();
                        }
                    }
                }

                break;

            case FdoPropertyType_ObjectProperty:
                objectPropertyDefinition = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                //
                // Only Value type is handled. Collection and Ordered collections should be handled as separate object
                if( objectPropertyDefinition->GetObjectType() == FdoObjectType_Value )
                {
                    const FdoSmLpPropertyMappingDefinition* mappping = objectPropertyDefinition->RefMappingDefinition();
                    if( mappping != NULL )
                    {
                        wchar_t *tmpStr = makePropName( scope, propertyDefinition->GetName() );
                        wchar_t* newScope = new wchar_t[wcslen(tmpStr) + 1 ];
                        if( newScope == NULL )
                            throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));

                        wcscpy( newScope, tmpStr );

                        switch(  mappping->GetType() )
                        {
                            case FdoSmLpPropertyMappingType_Concrete:
                                {
                                const FdoSmLpPropertyMappingConcrete * concMapping = static_cast<const FdoSmLpPropertyMappingConcrete*>( mappping );
                                AppendProperties( newScope, concMapping->RefTargetClass(), false, bGetAutoGen, propertyValues );
                                }
                                break;
                            case FdoSmLpPropertyMappingType_Single:
                                {
                                const FdoSmLpPropertyMappingSingle * singleMapping = static_cast<const FdoSmLpPropertyMappingSingle*>( mappping );
                                AppendProperties( newScope, singleMapping->RefTargetClass(), false, bGetAutoGen, propertyValues );
                                }
                                break;
                            default:
                                delete[] newScope;
                                throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));
                        }
                        delete[] newScope;
                    }
                }
                break;

            case FdoPropertyType_GeometricProperty:
                // Geometric properties are never auto-generated
                if ( !bGetAutoGen )
                {
                    geometryPropertyDefinition = static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);
                    if (geometryPropertyDefinition->GetIsSystem() == false &&
                        geometryPropertyDefinition->GetReadOnly() == false  )
                    {
                        propValue = FdoPropertyValue::Create( geometryPropertyDefinition->GetName(), NULL );
                        propertyValues->Add( propValue );
                        propValue->Release();
                    }
                }
                break;

                case FdoPropertyType_AssociationProperty:
                associationPropertyDefinition = static_cast<const FdoSmLpAssociationPropertyDefinition*>(propertyDefinition);
                // Association identity properties are never auto-generated.
                if( !bGetAutoGen && ! associationPropertyDefinition->GetReadOnly() )
                {
                    const FdoStringsP   identCol = associationPropertyDefinition->GetIdentityProperties();
                    if( identCol->GetCount() != 0 )
                    {
                        //
                        // We add to the list the association fk properties as <property name>.<associated class fk property>
                        // This new entry will create a "sort of" duplicate entry in the property list. These entries and the reverse
                        // identity properties are the same; Their values, if provided, should be identical.
                        for( int i=0; i<identCol->GetCount(); i++ )
                        {
                            FdoStringP identName = identCol->GetString(i);
                            propValue = FdoPropertyValue::Create();
                            FdoStringP propName = FdoStringP::Format(L"%ls.%ls",propertyDefinition->GetName(), (const wchar_t*)identName);
                            wchar_t *tmpStr = makePropName( scope,(const wchar_t*)propName );
                            propValue->SetName( tmpStr );
                            propertyValues->Add(propValue);
                            propValue->Release();
                        }
                    }
                    else
                    {
                        // Identity property not provided,
                        // use the default where the associated class identity properties are used as fk properties.
                        const FdoSmLpClassDefinition* associatedClass = associationPropertyDefinition->RefAssociatedClass();
                        const FdoSmLpDataPropertyDefinitionCollection*  propCollection = associatedClass->RefIdentityProperties();
                        for(int i=0; i<propCollection->GetCount(); i++ )
                        {
                            FdoPropertyValue* propValue = FdoPropertyValue::Create();
                            FdoStringP propName = FdoStringP::Format(L"%ls.%ls",propertyDefinition->GetName(), (const wchar_t*)(propCollection->RefItem(i)->GetName()));
                            wchar_t *tmpStr = makePropName( scope, (const wchar_t*)propName );
                            propValue->SetName( tmpStr );
                            propertyValues->Add(propValue);
                            propValue->Release();
                        }
                    }
                }
                break;

            default:
                throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_53, "Unsupported Schema Property Type"));
        }

    }
}

const FdoSmLpSchema* FdoRdbmsSchemaUtil::GetSchema(const wchar_t *className)
{
    int i;
    const FdoSmLpSchemaCollection* logPhysSchemas = mSchemaMgr->RefLogicalPhysicalSchemas();
    const FdoSmLpSchema *schema = NULL;

    FdoPtr<FdoIdentifier> clssIdentifier = FdoIdentifier::Create( className );
    const wchar_t *schemaName = clssIdentifier->GetSchemaName();
    if( schemaName && schemaName[0] != '\0' )
    {
       schema = logPhysSchemas->RefItem( schemaName );
    }
    else
    {
        int  len;
        const wchar_t *name;
        const wchar_t **scopes = clssIdentifier->GetScope( len );
        if( len != 0 )
            name = scopes[0];
        else
            name = clssIdentifier->GetName();

        for (i=0; i<logPhysSchemas->GetCount(); i++)
        {
            schema = logPhysSchemas->RefItem(i);
            if (schema->RefClasses()->RefItem(name))
            {
                break;
            }
        }
        if (i == logPhysSchemas->GetCount())
            schema = NULL;
    }
    if (schema == NULL)
        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_60, "Schema not found for class '%1$ls'", className));

    return schema;
}

const wchar_t* FdoRdbmsSchemaUtil::Property2ColName(const wchar_t *className, const wchar_t *attribute)
{

    const wchar_t* string = NULL;

    const FdoSmLpClassDefinition *classDefinition = GetClass(className);
    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();
    for (int i=0; i<propertyDefinitions->GetCount(); i++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(i);

        if (wcscmp(propertyDefinition->GetName(), attribute) == 0)
        {
            if (propertyDefinition->GetPropertyType() == FdoPropertyType_DataProperty)
            {
                const FdoSmLpDataPropertyDefinition* dataProp =
                    static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                const FdoSmPhColumn *column = dataProp->RefColumn();
                string = column->GetName();
            }
            break;
        }
    }
    return string;
}
const wchar_t *FdoRdbmsSchemaUtil::ColName2Property(const wchar_t *className, const wchar_t *columnName)
{
    int i;
    const wchar_t* string = NULL;

    const FdoSmLpClassDefinition *classDefinition = GetClass(className);
    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();
    for (i=0; NULL==string && i<propertyDefinitions->GetCount(); i++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(i);


        if (propertyDefinition->GetPropertyType() == FdoPropertyType_DataProperty ||
            propertyDefinition->GetPropertyType() == FdoPropertyType_GeometricProperty )
        {
            const FdoSmLpSimplePropertyDefinition* simpleProp =
                static_cast<const FdoSmLpSimplePropertyDefinition*>(propertyDefinition);

            const FdoSmPhColumn *column = simpleProp->RefColumn();

            if (NULL != column)
            {
                const wchar_t *colName = column->GetName();
                if (FdoCommonOSUtil::wcsicmp(colName, columnName) == 0)
                {
                    return propertyDefinition->GetName();
                }
            }
            if (NULL==string && propertyDefinition->GetPropertyType() == FdoPropertyType_GeometricProperty)
            {
                // Check other columns that may be associated with a geometric property.
                // Probably none other than the X ordinate column will ever come through here,
                // but we'll allow any of them anyway (X, Y, Z, SI1, SI2).
                const FdoSmLpGeometricPropertyDefinition* geometricProp =
                    static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);
                FdoString *colNameX = geometricProp->GetColumnNameX();
                FdoString *colNameY = geometricProp->GetColumnNameY();
                FdoString *colNameZ = geometricProp->GetColumnNameZ();
                FdoString *colNameSi1 = geometricProp->GetColumnNameSi1();
                FdoString *colNameSi2 = geometricProp->GetColumnNameSi2();

                if ( ( NULL != colNameX && FdoCommonOSUtil::wcsicmp(colNameX, columnName) == 0 ) ||
                     ( NULL != colNameY && FdoCommonOSUtil::wcsicmp(colNameY, columnName) == 0 ) ||
                     ( NULL != colNameZ && FdoCommonOSUtil::wcsicmp(colNameZ, columnName) == 0 ) ||
                     ( NULL != colNameSi1 && FdoCommonOSUtil::wcsicmp(colNameSi1, columnName) == 0 ) ||
                     ( NULL != colNameSi2 && FdoCommonOSUtil::wcsicmp(colNameSi2, columnName) == 0 ) )
                {
                    return propertyDefinition->GetName();
                }
            }
        }
        else
        {
            continue;
        }
    }
    //if( NULL == string )
        throw FdoSchemaException::Create(NlsMsgGet2(FDORDBMS_406, "Property '%1$ls' from class '%2$ls' has no database mapping", columnName, className));
    //return string;
}

const wchar_t *FdoRdbmsSchemaUtil::GetTable(const wchar_t *className)
{
    const char* string = NULL;

    const FdoSmLpClassDefinition *classDefinition = GetClass(className);

    const FdoSmLpDbObject* table = classDefinition->RefDbObject();
    if (table == NULL) {
        const FdoSmLpDataPropertyDefinitionCollection *identProperties = classDefinition->RefIdentityProperties();
        if(identProperties->GetCount() == 0 )
            throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_34, "Table name or Primary key for class '%1$ls' does not exist", className));
        else
            throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_262, "Table for class '%1$ls' does not exist", className));
    }

    return table->GetName();
}


// checks if table exists
void FdoRdbmsSchemaUtil::CheckClass(const wchar_t* className)
{
    if (className)
    {
        const FdoSmLpClassDefinition *classDefinition = this->GetClass(className);
        if (classDefinition->RefIdentityProperties()->GetCount() == 0)
            throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_266, "Creating/Updating a standalone instance for class '%1$ls' is not allowed", className));
        if (classDefinition->RefDbObject() == NULL)
            throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_265, "Table does not exist for class '%1$ls'", className));
    }
}

FdoStringP FdoRdbmsSchemaUtil::GetDbObjectSqlName( const FdoSmLpClassDefinition *classDefinition )
{
    const FdoSmLpDbObject* lpObject = classDefinition->RefDbObject();

    if (lpObject == NULL)
        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_265, "Table does not exist for class '%1$ls'", (FdoString*)(classDefinition->GetQName())));

    return lpObject->RefDbObject()->GetDbQName();
}

FdoStringP FdoRdbmsSchemaUtil::GetDbObjectSqlName( const FdoSmLpObjectPropertyDefinition *propertyDefinition )
{
    const FdoSmPhDbObject* phObject = propertyDefinition->RefContainingDbObject();

    if (phObject == NULL)
        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_268, "Table does not exist for object property '%1$ls'", (FdoString*)(propertyDefinition->GetQName())));

    return phObject->GetDbQName();
}

FdoStringP FdoRdbmsSchemaUtil::GetColumnSqlName( const FdoSmLpSimplePropertyDefinition *propertyDefinition )
{
    const FdoSmPhColumn* phColumn = propertyDefinition->RefColumn();

    if (phColumn == NULL)
        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_267, "Column does not exist for property '%1$ls'", (FdoString*)(propertyDefinition->GetQName())));

    return phColumn->GetDbName();
}



////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsSchemaUtil::CheckGeomPropOrdDimensionality( const FdoSmLpClassDefinition *classDefinition,
                                                      const wchar_t*    geomPropName,
                                                      FdoIGeometry      *pGeomValue )
{
    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();
    const FdoSmLpPropertyDefinition           *propertyDefinition = propertyDefinitions->RefItem(geomPropName);

    if ( propertyDefinition )
    {
        if  ( propertyDefinition->GetPropertyType() == FdoPropertyType_GeometricProperty )
        {
            const FdoSmLpGeometricPropertyDefinition* geomProp =
                static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);

            int             geomDimens = pGeomValue->GetDimensionality();
            const wchar_t   *phTableName = geomProp->RefContainingDbObject()->GetName();
            FdoRdbmsUtil     util;
#pragma message ("ToDo: FdoRdbmsSchemaUtil::CheckGeomPropOrdDimensionality")
            if ( false/*TODO strncmp( util.UnicodeToUtf8(phTableName), ADB_GEOMETRY_0_TABLE, strlen(ADB_GEOMETRY_0_TABLE)-1) == 0*/)
            {
                // Cannot Insert geometries with M in ADB_GEOMETRY_TABLE_<n>
                // The dimensionality is fixed to XYZ. The XY geometries will be padded.
                if ( geomDimens & FdoDimensionality_M )
                    throw FdoCommandException::Create(NlsMsgGet2(FDORDBMS_329, "Ordinate dimensionality mismatch on geometric property '%1$ls' of the feature class '%2$ls'. Only XY and XYZ allowed",
                            propertyDefinition->GetName(),
                            classDefinition->GetName()));
            }
            else
            {
                // The geometries in the class table should match exactly as dimensionality
                bool    bPropHasZ = geomProp->GetHasElevation();
                bool    bPropHasM = geomProp->GetHasMeasure();
                bool    bGeomHasZ = (geomDimens & FdoDimensionality_Z ) != 0;
                bool    bGeomHasM = (geomDimens & FdoDimensionality_M ) != 0;
#pragma message ("ToDo: FdoRdbmsSchemaUtil::CheckGeomPropOrdDimensionality: check for capability for dimentionality support")
                return;
                if ( bPropHasZ != bGeomHasZ || bPropHasM != bGeomHasM )
                    throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_330, "Ordinate dimensionality mismatch on geometric property '%1$ls",
                            propertyDefinition->GetName()));
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsSchemaUtil::AssociatedSpatialContextCheck( const FdoSmLpClassDefinition *pClassDefinition,
                                                        const wchar_t *geomPropName )
{
/* TODO:
    dbi_plan_info_def   act_plan_info;
    DbiConnection       *pDbiConn = mSchemaMgr->GetConnection();
    dbi_pn_id_t         act_plan_num = pDbiConn->dbi_plan_active_get();
    int                 found = false;

    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = pClassDefinition->RefProperties();
    const FdoSmLpPropertyDefinition *pPropertyDefinition = propertyDefinitions->RefItem(geomPropName);
    const FdoSmLpGeometricPropertyDefinition* pGeometricProperty =
                            dynamic_cast<const FdoSmLpGeometricPropertyDefinition*>(pPropertyDefinition);

    FdoString  *assocSC = pGeometricProperty->GetSpatialContextAssociation();

    pDbiConn->dbi_plan_get( act_plan_num, &act_plan_info, &found );

    if ( strcmp( pDbiConn->GetUtility()->UnicodeToUtf8(assocSC), act_plan_info.name ) != 0 )
        throw FdoSpatialContextMismatchException::Create(
            NlsMsgGet4( FDORDBMS_370,
                "The spatial context '%1$ls' of the geometry property '%2$ls' of the feature class '%3$ls' does not match the active spatial context '%4$ls'",
                (FdoString *)assocSC,
                pGeometricProperty->GetName(),
                pClassDefinition->GetName(),
                (FdoString *) FdoStringP( act_plan_info.name ) )
            );
*/
}

////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsSchemaUtil::SetActiveSpatialContext( const FdoSmLpClassDefinition *pClassDefinition,
                                                  const wchar_t *geomPropName )
{
    if ( pClassDefinition == NULL ||
         ( pClassDefinition && pClassDefinition->GetClassType() != FdoClassType_FeatureClass ))
        return;

    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = pClassDefinition->RefProperties();
    const FdoSmLpPropertyDefinition *pGeomPropertyDefinition;

    if ( geomPropName == NULL )
    {
        const FdoSmLpFeatureClass*  pFeatClass = static_cast<const FdoSmLpFeatureClass *>( pClassDefinition );
        pGeomPropertyDefinition = pFeatClass->RefGeometryProperty();
    }
    else
    {
        pGeomPropertyDefinition = propertyDefinitions->RefItem(geomPropName);
    }

    // May be missing
    if( pGeomPropertyDefinition == NULL )
        return;

    const FdoSmLpGeometricPropertyDefinition* pGeometricProperty =
                            dynamic_cast<const FdoSmLpGeometricPropertyDefinition*>(pGeomPropertyDefinition);

    // Change the active plan
    if ( pGeometricProperty )
    {
/* TODO:
        DbiConnection       *pDbiConn = mSchemaMgr->GetConnection();
        dbi_plan_info_def   plan_info;
        int                 found = false;
        long                oldActiveSC;
        long                newActiveSC;

        FdoStringP          assocSC = pGeometricProperty->GetSpatialContextAssociation();

        // Find the plan number by name
        pDbiConn->dbi_plan_find( (char*)(const char*)assocSC, &plan_info, &found );

        if ( !found )
            throw FdoSpatialContextMismatchException::Create(
                NlsMsgGet1( FDORDBMS_322,
                    "Spatial context '%1$ls' not found", (FdoString *) assocSC)
                );

        oldActiveSC = pDbiConn->dbi_plan_active_get();
        newActiveSC = plan_info.plan;

        if ( oldActiveSC != newActiveSC )
            pDbiConn->dbi_plan_active_set( newActiveSC );
*/
    }
}

////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsSchemaUtil::CheckGeomPropShapeDimensionality( const FdoSmLpClassDefinition *classDefinition,
                                                      const wchar_t*    geomPropName,
                                                      FdoIGeometry      *pGeomValue )
{
    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();
    const FdoSmLpPropertyDefinition           *propertyDefinition = propertyDefinitions->RefItem(geomPropName);

    // Fast return if nothing to do.
    if ( propertyDefinition == NULL ||
        propertyDefinition->GetPropertyType() != FdoPropertyType_GeometricProperty)
        return;

    const FdoSmLpGeometricPropertyDefinition* geomProp =
        static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);

    int                 allowedShapeTypes = geomProp->GetGeometryTypes();
    FdoGeometryType     geomType = pGeomValue->GetDerivedType();

    bool    bIsPoint   = ( geomType == FdoGeometryType_Point        || geomType == FdoGeometryType_MultiPoint);
    bool    bIsCurve   = ( geomType == FdoGeometryType_LineString   || geomType == FdoGeometryType_MultiLineString ||
                           geomType == FdoGeometryType_CurveString  || geomType == FdoGeometryType_MultiCurveString );
    bool    bIsPolygon = ( geomType == FdoGeometryType_Polygon      || geomType == FdoGeometryType_MultiPolygon ||
                           geomType == FdoGeometryType_CurvePolygon || geomType == FdoGeometryType_MultiCurvePolygon );
    bool    bIsMulGeom = ( geomType == FdoGeometryType_MultiGeometry );

    if (    bIsPoint   && ((allowedShapeTypes & FdoGeometricType_Point) == 0) ||
            bIsCurve   && ((allowedShapeTypes & FdoGeometricType_Curve) == 0) ||
            bIsPolygon && ((allowedShapeTypes & FdoGeometricType_Surface) == 0) ||
            bIsMulGeom && ((allowedShapeTypes & FdoGeometricType_All) == 0) )
    {
        throw FdoCommandException::Create(NlsMsgGet2(
                FDORDBMS_390,
                "Shape dimensionality mismatch on geometric property '%1$ls' of the feature class '%2$ls'",
                propertyDefinition->GetName(),
                classDefinition->GetName()));
    }

}

void FdoRdbmsSchemaUtil::SanitizePropertyValues( const FdoSmLpClassDefinition *classDefinition,
                                                 FdoPropertyValueCollection      *propValCollection,
                                                 bool                            *containsObjectProperties )
{
    *containsObjectProperties = false;

    const FdoSmLpPropertyDefinitionCollection *properties = classDefinition->RefProperties();
    for(int i=0; i<propValCollection->GetCount(); i++ )
    {
        FdoPtr<FdoPropertyValue>propVal = propValCollection->GetItem(i);
        FdoPtr<FdoIdentifier>proId = propVal->GetName();
        const FdoSmLpPropertyDefinition *prop = properties->RefItem( proId->GetText() );
        if( prop == NULL )
        {
            FdoInt32 length;
            FdoString** scopes = proId->GetScope(length);
            if (length == 0)
               throw FdoCommandException::Create( NlsMsgGet1( FDORDBMS_56, "Property '%1$ls' not found", proId->GetText() ) );
            continue;
        }

        if( prop->GetPropertyType() == FdoPropertyType_DataProperty )
        {
            const FdoSmLpDataPropertyDefinition *dataProp = (FdoSmLpDataPropertyDefinition *)prop;
            if(  ! mSkipCheck && (dataProp->GetIsSystem() ||  dataProp->GetIsAutoGenerated() )  )
            {
               throw FdoCommandException::Create( NlsMsgGet1( FDORDBMS_393, "Property %1$ls is not user modifiable", proId->GetText() ) );
            }
        }
        else if( prop->GetPropertyType() == FdoPropertyType_ObjectProperty )
                 *containsObjectProperties = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsSchemaUtil::CheckGeomPropValidity( const FdoSmLpClassDefinition *classDefinition,
                                                      const wchar_t*    geomPropName,
                                                      FdoIGeometry      *pGeomValue )
{
    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();
    const FdoSmLpPropertyDefinition           *propertyDefinition = propertyDefinitions->RefItem(geomPropName);

    // Fast return if nothing to do.
    if ( propertyDefinition == NULL ||
        propertyDefinition->GetPropertyType() != FdoPropertyType_GeometricProperty)
        return;

    const FdoSmLpGeometricPropertyDefinition* geomProp =
        static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);

    FdoGeometryType     geomType = pGeomValue->GetDerivedType();
//    dbi_plan_info_def   plan_info;
    bool                geomIsValid = true;
//    int                 found;

    // Quick return if type not handled
    if ( geomType == FdoGeometryType_Point ||
         geomType == FdoGeometryType_MultiPoint ||
         geomType == FdoGeometryType_LineString ||
         geomType == FdoGeometryType_MultiLineString ||
         geomType == FdoGeometryType_Polygon ||
         geomType == FdoGeometryType_MultiPolygon )
    {
        return;
    }
/* TODO:
    // Find the spatial context definition by name
    FdoStringP  assocSC = geomProp->GetSpatialContextAssociation();
    mSchemaMgr->GetConnection()->dbi_plan_find( (char*)(const char*)assocSC, &plan_info, &found );

    if ( !found )
    {
        throw FdoSpatialContextMismatchException::Create(
            NlsMsgGet1( FDORDBMS_322,
                "Spatial context '%1$ls' not found", (FdoString *) assocSC)
            );
    }

    double xyResol = plan_info.storage_resolution/2;

    switch ( geomType )
    {
        case FdoGeometryType_CurveString:
        {
            FdoICurveString* geom = (FdoICurveString *)pGeomValue;
            geomIsValid = IsCurveStringValid( geom, xyResol );
            break;
        }
        case FdoGeometryType_MultiCurveString:
        {
            FdoIMultiCurveString *geom = (FdoIMultiCurveString *)pGeomValue;
            for ( FdoInt32 i = 0; i < geom->GetCount() && geomIsValid; i++ )
            {
                FdoICurveString *curve = (FdoICurveString *)geom->GetItem(i);
                geomIsValid = IsCurveStringValid( curve, xyResol );
                FDO_SAFE_RELEASE( curve );
            }
            break;
        }

        case FdoGeometryType_CurvePolygon:
        {
            FdoICurvePolygon *geom = (FdoICurvePolygon *)pGeomValue;
            geomIsValid = IsCurvePolygonValid( geom, xyResol );
            break;
        }
        case FdoGeometryType_MultiCurvePolygon:
        {
            FdoIMultiCurvePolygon* geom = (FdoIMultiCurvePolygon *)pGeomValue;
            for ( FdoInt32 i = 0; i < geom->GetCount() && geomIsValid; i++ )
            {
                FdoICurvePolygon *poly = (FdoICurvePolygon *)geom->GetItem(i);
                geomIsValid = IsCurvePolygonValid( poly, xyResol );
                FDO_SAFE_RELEASE( poly );
            }
            break;
        }
        case FdoGeometryType_MultiGeometry:
        {
            break;
        }
    default:
        break;
    }

    if ( !geomIsValid )
    {
        throw FdoRdbmsException::Create(NlsMsgGet2(
                FDORDBMS_401,
                "Invalid geometry value on geometric property '%1$ls' of the feature class '%2$ls'. It contains arc(s) with collinear points.",
                propertyDefinition->GetName(),
                classDefinition->GetName()));
    }
*/
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool FdoRdbmsSchemaUtil::IsCurveStringValid( FdoICurveString *curve, double xyResol )
{
    bool    arcIsValid = true;

    for ( FdoInt32 i = 0; i < curve->GetCount() && arcIsValid; i++ )
    {
        FdoICurveSegmentAbstract *seg = curve->GetItem(i);
        if ( seg->GetDerivedType() == FdoGeometryComponentType_CircularArcSegment )
        {
            FdoICircularArcSegment *arc = (FdoICircularArcSegment *)seg;
            arcIsValid = FdoSpatialUtility::IsCircularArcValid( arc, xyResol );
        }
        FDO_SAFE_RELEASE(seg);
    }

    return arcIsValid;
}

bool FdoRdbmsSchemaUtil::IsCurvePolygonValid( FdoICurvePolygon *poly, double xyResol )
{
    bool    arcIsValid = true;

    FdoPtr<FdoIRing> extRing = poly->GetExteriorRing();
    arcIsValid = IsRingValid( extRing, xyResol );

    for ( FdoInt32 i = 0; i < poly->GetInteriorRingCount() && arcIsValid; i++ )
    {
        FdoPtr<FdoIRing>  intRing = poly->GetInteriorRing(i);
        arcIsValid = IsRingValid( intRing, xyResol );
    }

    return arcIsValid;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool FdoRdbmsSchemaUtil::IsRingValid( FdoIRing *ring, double xyResol )
{
    bool    arcIsValid = true;

    for ( FdoInt32 i = 0; i < ring->GetCount() && arcIsValid; i++ )
    {
        FdoICurveSegmentAbstract *seg = ring->GetItem(i);
        if ( seg->GetDerivedType() == FdoGeometryComponentType_CircularArcSegment )
        {
            FdoICircularArcSegment *arc = (FdoICircularArcSegment *)seg;
            arcIsValid = FdoSpatialUtility::IsCircularArcValid( arc, xyResol );
        }
        FDO_SAFE_RELEASE(seg);
    }

    return arcIsValid;
}
