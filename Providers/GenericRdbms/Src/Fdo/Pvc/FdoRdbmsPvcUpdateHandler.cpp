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
#include "FdoCommonOSUtil.h"
#include "FdoRdbmsConnection.h"
#include "Fdo/Pvc/FdoRdbmsPvcUpdateHandler.h"
#include "Fdo/Pvc/FdoRdbmsPropBindHelper.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsLobUtility.h"
#include "ut.h"

#define UPDATE_CLEANUP2 \
        if ( statement != NULL ) \
        { \
            delete statement; \
            statement = NULL; \
        } \
        if ( duplicate != NULL ) \
        { \
            delete [] duplicate; \
            duplicate = NULL; \
        }

// If there are spatial index columns for a geometric property, there are exactly
// this number to bind (some may have empty values).
#define FIXED_NUM_SI_COLUMNS    (2)

FdoRdbmsPvcUpdateHandler::FdoRdbmsPvcUpdateHandler( ):mFdoConnection(NULL)
{
}

FdoRdbmsPvcUpdateHandler::FdoRdbmsPvcUpdateHandler( FdoRdbmsConnection *connection ):
mFdoConnection( connection )// no addref as it is owned by a component that is already holding a reference on the connection. 
{
}

FdoRdbmsPvcUpdateHandler::~FdoRdbmsPvcUpdateHandler()
{
}

long FdoRdbmsPvcUpdateHandler::Execute( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection, bool revisionNumberUpdate, bool handleForeignAutoincrementedId )
{
    int                 numberOfRows = 0;
    int                 skipIndex = -1;
    GdbiStatement       *statement = NULL;
    bool                isFeatClass = (classDefinition->GetClassType() == FdoClassType_FeatureClass);
	DbiConnection		*mConnection = mFdoConnection->GetDbiConnection();
	FdoRdbmsSpatialManagerP spatialManager = mFdoConnection->GetSpatialManager();
    int                 i;
    int                 j;
    bool                *duplicate = NULL;
    bool                hasLobsByRef = false;
    FdoRdbmsPropBindHelper bindHelper(mFdoConnection);
    std::vector< std::pair< FdoLiteralValue*, FdoInt64 > > bindProps;

    FdoPtr<FdoDataValueCollection> specialValues = FdoDataValueCollection::Create();
    int                 count = 0;

    //
    // Update the atributes using a command of the following form: update tab set col1=val1,col2=val2.. where id in (select id from tab where ...)
    const FdoSmLpDbObject* table = classDefinition->RefDbObject();
    FdoStringP tableName = table->RefDbObject()->GetDbQName();
	FdoStringP updateString;
    const FdoSmLpDataPropertyDefinition *pDef = FdoSmLpDataPropertyDefinition::Cast(classDefinition->RefSystemProperty(L"RevisionNumber"));
    if ( (pDef != NULL) && (pDef->RefColumn() == NULL) )
        pDef = NULL;

	if (pDef && revisionNumberUpdate)
		updateString = FdoStringP::Format(L"update %ls set revisionnumber = revisionnumber+1, ", (FdoString *) tableName );
	else
		updateString = FdoStringP::Format(L"update %ls set ", (FdoString*) tableName );

    bool updateProperties = false;
    bool first = true;

    FdoSmLpDataPropertyDefinitionCollection featIdProps;
    const FdoSmLpDataPropertyDefinitionCollection *properties = NULL;

    const FdoSmLpDataPropertyDefinition *featIdProp = isFeatClass ? classDefinition->RefFeatIdProperty() : NULL;
    const FdoSmLpGeometricPropertyDefinition *pGeometryProp = NULL;

    properties = classDefinition->RefIdentityProperties();

    if( properties->GetCount() == 0 )
        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_45, "Schema error; no primary key found"));


    int  bindIndex = 1;
    int  bindIndexRet = 0;
    count = propValCollection->GetCount();

    // Get how many geometry properties in the feature class.
    const FdoSmLpPropertyDefinitionCollection * propertiesFromClassDef = classDefinition->RefProperties();
    int iTotalCount = propertiesFromClassDef->GetCount();
    int iGeomCount = 0;
    for(int k = 0; k<iTotalCount; k++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = propertiesFromClassDef->RefItem(k);
        if(NULL != propertyDefinition)
        {
            if(FdoPropertyType_GeometricProperty == propertyDefinition->GetPropertyType())
			    iGeomCount++;
		}
    }

	if( spatialManager != NULL )
		count += FIXED_NUM_SI_COLUMNS * iGeomCount ; // Geometry property may require up to 2 extra columns
	
    duplicate = new bool[count];
    for (i=0;i<count; i++)
    {
        duplicate[i] = false;
        for (j=i+1; j<propValCollection->GetCount(); j++)
        {
            FdoPtr<FdoPropertyValue> propertyValue = propValCollection->GetItem(i);
            FdoPtr<FdoIdentifier> id = propertyValue->GetName();
            FdoPtr<FdoPropertyValue> propertyValue2 = propValCollection->GetItem(j);
            FdoPtr<FdoIdentifier> id2 = propertyValue2->GetName();
            if (wcscmp(id->GetText(), id2->GetText()) == 0)
            {
                duplicate[i] = true;
                break;
            }
        }
    }

    // The property value collection may include a value for the class feature id
    // columns. If this column is not the class primary key, then it needs to be
    // ignored in the following process building the update statement to be executed
    // as a feature id property cannot be updated. This case may happen if the class
    // represents a feature class with multiple identity columns.
    const wchar_t *prmPrtName = L"FeatId";
    if ( isFeatClass && featIdProp )
        prmPrtName = featIdProp->GetName();
    try
    {
        for (i=0; i<propValCollection->GetCount(); i++)
        {
            if (duplicate[i] == true)
                continue;
            FdoPtr<FdoPropertyValue> propertyValue = propValCollection->GetItem(i);
            if ( propertyValue == NULL )
                throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_39, "Property value is NULL"));
            FdoPtr<FdoIdentifier>id = propertyValue->GetName();
            const wchar_t* name  = id->GetText();

            FdoPtr<FdoValueExpression> literalExpression = propertyValue->GetValue();
            FdoPtr<FdoIStreamReader> streamReader = propertyValue->GetStreamReader();

            // Skip the streamed properties for now ...
            if ( literalExpression == NULL && streamReader == NULL)
                continue;  // Must not have been set

            for( j =0; properties!= NULL && j<properties->GetCount(); j++ )
            {
                if ( wcscmp(name, properties->RefItem(j)->GetName() ) == 0 )
                {
                    break;
                }
            }
            if( j < properties->GetCount() )
            {
                continue; // This is a primary key column; should appear in the where clause as: where prim_col1 = :1 and prim_col2= :2 ...
            }

            // Filter out the class feature id property if in the property value collection
            // and not eliminated as a primary key.

            if ( isFeatClass )
            {
                if ( wcscmp(name, prmPrtName ) == 0 )
                {
                    skipIndex = i;
                    continue;
                }
            }

            FdoStringP colName;
            const FdoSmLpPropertyDefinition *propertyDefinition = classDefinition->RefProperties()->RefItem(name);
            FdoPropertyType propType = FdoPropertyType_DataProperty;

            if( propertyDefinition != NULL )
            {
               propType = propertyDefinition->GetPropertyType();
               if( propType == FdoPropertyType_DataProperty )
               {
                   const FdoSmLpDataPropertyDefinition* dataProp =
                        static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                    const FdoSmPhColumn *column = dataProp->RefColumn();
                    colName = column->GetDbName();
               }
               else if ( propType == FdoPropertyType_GeometricProperty)
               {
                    const FdoSmLpGeometricPropertyDefinition* geomProp =
                        static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);

				    FdoSmOvGeometricColumnType columnType = geomProp->GetGeometricColumnType();
				    if( columnType != FdoSmOvGeometricColumnType_Double )
				    {
					    const FdoSmPhColumn *column = geomProp->RefColumn();
					    colName = column->GetDbName();
				    }
				    else
				    {
					    const FdoSmPhColumn *columnX = geomProp->RefColumnX();
					    if( columnX != NULL )
						    colName = columnX->GetDbName();

					    // We'll handle the Y and Z below
				    }
               }
            }
            else
            {
                // This must be an association property or single mapped object property
                int  length;
                const FdoString **scopes = id->GetScope( length );
                const FdoString *propName = id->GetName();
                bool  bSkip = false;
                if( length != 0 )
                {
                    const FdoSmLpPropertyDefinition *propertyDefinition = classDefinition->RefProperties()->RefItem( scopes[0] );
                    if( propertyDefinition->GetPropertyType() == FdoPropertyType_ObjectProperty )
                    {
                        const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                        const FdoSmLpPropertyMappingDefinition* mappping = objProp->RefMappingDefinition();
                        if( mappping->GetType() != FdoSmLpPropertyMappingType_Single )
                            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type")); // Should never get here
                        const FdoSmLpClassDefinition* currentClass = objProp->RefTargetClass();
                        for( int k=1;k<length;k++ )
                        {
                            propertyDefinition = classDefinition->RefProperties()->RefItem( scopes[k] );
                            objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                            mappping = objProp->RefMappingDefinition();
                            if( mappping->GetType() != FdoSmLpPropertyMappingType_Single )
                                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type")); // Should never get here

                            currentClass = objProp->RefTargetClass();
                        }
                        propertyDefinition = currentClass->RefProperties()->RefItem( propName );
                        if( propertyDefinition != NULL )
                        {
                            propType = propertyDefinition->GetPropertyType();
                            if( propType == FdoPropertyType_DataProperty )
                            {
                                const FdoSmLpDataPropertyDefinition* dataProp =
                                    static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                                const FdoSmPhColumn *column = dataProp->RefColumn();
                                colName = column->GetDbName();
                            }
                            else if ( propType == FdoPropertyType_GeometricProperty )
                            {
                                const FdoSmLpGeometricPropertyDefinition* geomProp =
                                    static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);
                                const FdoSmPhColumn *column = geomProp->RefColumn();
                                colName = column->GetDbName();
                            }
                            else
                            {
                                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error")); // Should never get here
                            }
                        }
                    }

                    else if( propertyDefinition->GetPropertyType() == FdoPropertyType_AssociationProperty && length == 1 )
                    {
                        const FdoSmLpAssociationPropertyDefinition* assocProp = static_cast<const FdoSmLpAssociationPropertyDefinition*>(propertyDefinition);
                        const FdoSmLpDataPropertyDefinitionCollection *identprop;
                        FdoSmLpDataPropertyDefinitionCollection props;
                        if( assocProp->GetIdentityProperties()->GetCount() == 0 )
                        {
                            // The name must be part of the associated class identity properties
                            identprop = assocProp->RefAssociatedClass()->RefIdentityProperties();
                        }
                        else
                        {
                            // This is one of the association identity properties
                            for(int i=0; i<assocProp->GetIdentityProperties()->GetCount(); i++ )
                            {
                                FdoStringP proName = assocProp->GetIdentityProperties()->GetString( i );
                                const FdoSmLpDataPropertyDefinition  *prop = (const FdoSmLpDataPropertyDefinition  *)assocProp->RefAssociatedClass()->RefProperties()->RefItem( proName );
                                props.Add( (FdoSmLpDataPropertyDefinition  *)prop );
                            }
                            identprop = &props;
                        }
                        for(int i=0; i<identprop->GetCount(); i++ )
                        {
                            if (FdoCommonOSUtil::wcsicmp((const wchar_t*)propName, identprop->RefItem(i)->GetName() ) == 0)
                            {
                                // Make sure the equivalent reverse identity property is not already set
                                if( assocProp->GetReverseIdentityProperties()->GetCount() != 0 )
                                {
                                    FdoStringP proName = assocProp->GetReverseIdentityProperties()->GetString( i );
                                    FdoPtr<FdoPropertyValue>propValue;
                                    try
                                    {
                                        propValue = propValCollection->GetItem((const wchar_t*)proName );
                                    }
                                    catch(FdoException *exp) {exp->Release();/* expected exception*/}

                                    if( propValue != NULL )
                                    {
                                        // Should check that both values match
                                        FdoPtr<FdoValueExpression>assoVal = propertyValue->GetValue();
                                        FdoPtr<FdoValueExpression>identVal = propValue->GetValue();
                                        if( assoVal != NULL && identVal != NULL && FdoCommonOSUtil::wcsicmp(assoVal->ToString(), identVal->ToString() ) )
                                            throw FdoCommandException::Create(NlsMsgGet2(FDORDBMS_291, "Association property '%1$ls' and property '%2$ls' must have the same value or only one should be set",
                                                            (const wchar_t*)name, (const wchar_t*)proName ));

                                        bSkip = true;
                                    }
                                }
                                colName = assocProp->GetReverseIdentityColumns()->GetDbString( i );
                                break;
                            }
                        }
                    }
                }
                if( bSkip )
                {
                    duplicate[i] = true;
                    continue;
                }
            }
            if( colName == L"" ) //TODO: Should never happen; May be it needs an exception
                colName =  mConnection->GetSchemaUtil()->Property2ColName( classDefinition->GetName(), name );

            if( ! first )
            {
                updateString += L", ";
            }
            first = false;
            updateProperties = true;
            updateString += colName ;
            updateString += L"=";

            if ( streamReader )
            {
                const FdoSmLpDataPropertyDefinition* dataProp =
                        static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);

                if ( dataProp->GetDataType() == FdoDataType_BLOB )
                {
                    hasLobsByRef = true;
                    updateString += L"EMPTY_BLOB()";
                    bindIndex++;
                    // do not increment bindIndex here
                }
            }
            else
            {
                bool isGeomProp = (propType == FdoPropertyType_GeometricProperty);
                updateString += mFdoConnection->GetBindString( bindIndex++, propertyDefinition );

			    if ( isGeomProp )
			    {
				    const FdoSmLpGeometricPropertyDefinition* geomProp =
                        static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);

				    FdoSmOvGeometricColumnType columnType = geomProp->GetGeometricColumnType();
				    if( columnType != FdoSmOvGeometricColumnType_Double )
				    {
					    const FdoSmPhColumn *columnSi1 = geomProp->RefColumnSi1();
					    const FdoSmPhColumn *columnSi2 = geomProp->RefColumnSi2();
					    if (NULL != columnSi1 && NULL != columnSi2)
					    {
						    updateString += L", ";
						    updateString += columnSi1->GetDbName() ;
						    updateString += L"=";
						    updateString += mFdoConnection->GetBindString( bindIndex++ );
						    updateString += L", ";
						    updateString += columnSi2->GetDbName() ;
						    updateString += L"=";
						    updateString += mFdoConnection->GetBindString( bindIndex++ );
					    }
				    }
				    else
				    {
					    // Handle the Y and possibly Z columns
					    const FdoSmPhColumn *columnY = geomProp->RefColumnY();
                        const FdoSmPhColumn *columnZ = geomProp->RefColumnZ();
                        if (NULL != columnY )
                        {
						    updateString += L", ";
						    updateString += columnY->GetDbName() ;
						    updateString += L"=";
						    updateString += mFdoConnection->GetBindString( bindIndex++ );
						    if( NULL != columnZ )
						    {
							    updateString += L", ";
							    updateString += columnZ->GetDbName() ;
							    updateString += L"=";
							    updateString += mFdoConnection->GetBindString( bindIndex++ );
						    }
					    }
				    }
			    }
            }
        }
    }
    catch(...)
    {
        delete [] duplicate;
        throw;
    }

    if( ! updateProperties )
    {
        delete [] duplicate;
        return numberOfRows;
    }

    for( i =0; properties!= NULL && i<properties->GetCount(); i++ )
    {
        if( i == 0 )
            updateString += L" where ";
        else
            updateString += L" and ";
        updateString += mConnection->GetSchemaUtil()->GetColumnSqlName(properties->RefItem(i));
        updateString += L" =";
        updateString += mFdoConnection->GetBindString( bindIndex++ );
    }

	AdditionalCriteria( propValCollection, updateString, bindIndex );

    try
    {
        statement = mConnection->GetGdbiConnection()->Prepare( (const wchar_t*) updateString );
         // FIXME: LOB stuff
        // if ( FdoRdbmsLobUtility::ContainsStreamedLobs( propValCollection ) )
        //     mConnection->dbi_option(qid, "define array size 1", NULL);

        //
        // Bind values to the bind variables
        bindIndex = 1;
		int propIdx = 0;
        for (i=0; i<count && propIdx < propValCollection->GetCount(); i++,propIdx++)
        {

            if (duplicate[i] == true)
                continue;

            if (skipIndex == i)
                continue;

            FdoPtr<FdoPropertyValue> propertyValue = propValCollection->GetItem(propIdx);
            if ( propertyValue == NULL )
                throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_39, "Property value is NULL"));
            FdoPtr<FdoIdentifier> id = propertyValue->GetName();
            const wchar_t* name  = id->GetText();

            FdoPtr<FdoValueExpression> literalExpression = propertyValue->GetValue();
            FdoPtr<FdoIStreamReader> streamReader = propertyValue->GetStreamReader();

            if ( literalExpression == NULL && streamReader == NULL )
                continue;  // Must not have been set

            int /*FdoDataType*/ dataType;

            FdoDataValue *dataValue = NULL;
            FdoGeometryValue*   geomValue = NULL;

            // Get the datatype
            if ( literalExpression )
            {
                int j;
                for( j =0; properties!= NULL && j<properties->GetCount(); j++ )
                {
                    if ( wcscmp(name, properties->RefItem(j)->GetName() ) == 0 )
                    {
                        break;
                    }
                }
                if( j < properties->GetCount() )
                    continue; // This is a primary key column; should appear in the where clause as: where prim_col1 = :1 and prim_col2= :2 ...

                geomValue = (dynamic_cast<FdoGeometryValue*>(literalExpression.p));

                if ( geomValue )
                {
                    dataType = FdoRdbmsDataType_Geometry;
                }
                else
                {
                    dataValue = (static_cast<FdoDataValue*>(literalExpression.p));
                    dataType = dataValue->GetDataType();
                }
            }
            else
            {
                // Streamed based
                const FdoSmLpPropertyDefinition *propertyDefinition = classDefinition->RefProperties()->RefItem(name);

                const FdoSmLpDataPropertyDefinition* dataProp =
                        static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                dataType = dataProp->GetDataType();
            }

            FdoStringP bindValue;
    
            switch ( dataType )
            {
                case FdoRdbmsDataType_Geometry:
                    {
						const FdoSmLpGeometricPropertyDefinition * geomPropDef = NULL;
						const FdoSmLpPropertyDefinitionCollection * propertyDefs = classDefinition->RefProperties();
						const FdoSmLpPropertyDefinition * propertyDef = propertyDefs->RefItem(name);
						if ( propertyDef == NULL )
                            throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_28, "Property '%1$ls' is not found", name));

						if ( FdoPropertyType_GeometricProperty != propertyDef->GetPropertyType() )
                            throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_486,
                                                                        "Expected property '%1$ls' to be a geometric property.",
                                                                        name));

						geomPropDef = static_cast<const FdoSmLpGeometricPropertyDefinition *>(propertyDef);

						FdoPtr<FdoFgfGeometryFactory>gf = FdoFgfGeometryFactory::GetInstance();
                        FdoPtr<FdoByteArray>ba = (static_cast<FdoGeometryValue*>(literalExpression.p))->GetGeometry();
                        int index = bindIndex-1;
						if (FdoSmOvGeometricColumnType_Double == geomPropDef->GetGeometricColumnType() )
						{
							if (ba == NULL)
								break;

							FdoPtr<FdoIGeometry>    geomValue = gf->CreateGeometryFromFgf( ba );
							FdoGeometryType         geomType = geomValue->GetDerivedType();

							FdoSmOvGeometricColumnType columnType = geomPropDef->GetGeometricColumnType();

							if (FdoSmOvGeometricColumnType_Double == columnType)
							{
								if (FdoGeometryType_Point != geomType)
									continue;
								FdoIPoint * pointValue = static_cast<FdoIPoint *>(geomValue.p);
		                            

								const FdoSmPhColumn *columnX = geomPropDef->RefColumnX();
								const FdoSmPhColumn *columnY = geomPropDef->RefColumnY();
								const FdoSmPhColumn *columnZ = geomPropDef->RefColumnZ();
								double x, y, z, m;
								FdoInt32 dimensionality;
								double doubleValue = 0.0;

								pointValue->GetPositionByMembers(&x, &y, &z, &m, &dimensionality);

								if (NULL != columnX )
								{
                                    FdoPtr<FdoDataValue> xValue = GetGeomOrdinateBindValue(x, columnX);
                                    specialValues->Add(xValue);
                                    bindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(xValue.p), 0));
								}
								if (NULL != columnY )
								{
                                    FdoPtr<FdoDataValue> yValue = GetGeomOrdinateBindValue(y, columnY);
                                    specialValues->Add(yValue);
                                    bindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(yValue.p), 0));
								}
								if (NULL != columnZ )
								{
                                    FdoPtr<FdoDataValue> zValue = GetGeomOrdinateBindValue(z, columnZ);
                                    specialValues->Add(zValue);
                                    bindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(zValue.p), 0));
								}
							}
							break;
						}
                        // implicit else
                        FdoIGeometry        *geom = NULL;
                        if ( ba )
                        {
                            geom = gf->CreateGeometryFromFgf(ba);

                            // Validate the input geometry
                            mConnection->GetSchemaUtil()->CheckGeomPropOrdDimensionality( classDefinition, name, geom );
                            mConnection->GetSchemaUtil()->CheckGeomPropShapeType( classDefinition, name, geom );
                            mConnection->GetSchemaUtil()->CheckGeomPropValidity( classDefinition, name, geom );

                        }

                        // The geom may be null for the feature with multiple geometries, but we still need to create the
                        // spatial index and bind them correctly.
						// if( geom != NULL )
						{
							// Set SRID for the geometry column
							const FdoSmPhColumnP gColumn = ((FdoSmLpSimplePropertyDefinition*)geomPropDef)->GetColumn();
							FdoSmPhColumnGeomP geomCol;
                            if (gColumn)
                                geomCol = gColumn.p->SmartCast<FdoSmPhColumnGeom>();
                            FdoInt64 srid = 0;
							if (geomCol)
                            {
								srid = geomCol->GetSRID();
                            }
                            bindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(literalExpression.p), (long)srid));

							FdoStringsP geomSiKeys;
								
							const FdoSmPhColumn *columnSi1 = geomPropDef->RefColumnSi1();
							const FdoSmPhColumn *columnSi2 = geomPropDef->RefColumnSi2();
							if (NULL != columnSi1 && NULL != columnSi2 && spatialManager != NULL )
							{
								spatialManager->InsertGeometryInLine(geomPropDef, geomValue, geomSiKeys);

								for (int n=0; n < FIXED_NUM_SI_COLUMNS; n++ )
								{
									index = bindIndex-1;
									i++;
									const wchar_t *wcharValue = L"";
                                    FdoPtr<FdoStringValue> siValue;
                                    if (geomSiKeys != NULL && n < geomSiKeys->GetCount())
                                        siValue = FdoStringValue::Create(geomSiKeys->GetString(n));
                                    else
                                        siValue = FdoStringValue::Create(L"");

                                    specialValues->Add(siValue);
                                    bindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(siValue.p), 0));
 								}
							}
						}
                    }
                    break;

                default:
                    bindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(literalExpression.p), 0));
                    break;
            }
        }

        delete [] duplicate;
        duplicate = NULL;

        // Primary keys (part of "where" clause)
        for(i =0; properties!= NULL && i<properties->GetCount(); i++ )
        {
            int index = bindIndex-1;

            // Initialize the bind variable
            const FdoSmLpDataPropertyDefinition *propertyDefinition = properties->RefItem(i);

            FdoPtr<FdoPropertyValue> propVal = propValCollection->GetItem( propertyDefinition->GetName() );
            FdoPtr<FdoValueExpression> literalExpression = propVal->GetValue();

            if ( literalExpression == NULL )
                continue;  // Must not have been set

            bindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(literalExpression.p), 0));
        }

        AdditionalBinds( propValCollection, &bindProps, bindIndex );

        bindHelper.BindParameters(mConnection->GetGdbiCommands(), statement->GetQueryId(), &bindProps);
        numberOfRows = statement->ExecuteNonQuery();

        // Stream Write ...
//        FdoRdbmsLobUtility::InsertStreamedLobs( mConnection, classDefinition, propValCollection, values, count );

        // Clean up
        delete statement;
        statement = NULL;

        return numberOfRows;
    }
    catch (FdoCommandException *ex)
    {
        ex;
        UPDATE_CLEANUP2;
        throw;
    }
    catch (FdoException *ex)
    {
        UPDATE_CLEANUP2;
        FdoCommandException *exp = FdoCommandException::Create(ex->GetExceptionMessage(), ex, ex->GetNativeErrorCode());
        ex->Release();
        throw exp;
    }
    catch ( ... )
    {
        UPDATE_CLEANUP2;
        throw;
    }
}

void FdoRdbmsPvcUpdateHandler::AdditionalCriteria( FdoPropertyValueCollection  *propValCollection, FdoStringP & updateString, int & bindIndex )
{
}

void FdoRdbmsPvcUpdateHandler::AdditionalBinds(  FdoPropertyValueCollection  *propValCollection, std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* params, int & bindIndex  )
{
}
