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
    FdoRdbmsPvcBindDef     *values = NULL;
    int                 count = 0;
    char                buffer[16];

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
	if( spatialManager != NULL )
		count += FIXED_NUM_SI_COLUMNS; // Geometry property may require up to 2 extra columns
	
    values = new FdoRdbmsPvcBindDef[count];
    for (i=0; i<count; i++)
    {
        mConnection->GetGdbiCommands()->set_null(&values[i].null_ind, 0,0);
        values[i].value.strvalue = NULL;
        values[i].reader = NULL;
        values[i].type = FdoDataType_String;
        values[i].propertyName[0] = '\0';
        values[i].name[0] = '\0';
		values[i].valueNeedsFree = false;
    }
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
                        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error")); // Should never get here
                    const FdoSmLpClassDefinition* currentClass = objProp->RefTargetClass();
                    for( int k=1;k<length;k++ )
                    {
                        propertyDefinition = classDefinition->RefProperties()->RefItem( scopes[k] );
                        objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                        mappping = objProp->RefMappingDefinition();
                        if( mappping->GetType() != FdoSmLpPropertyMappingType_Single )
                            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error")); // Should never get here

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

    if( ! updateProperties )
    {
        delete [] duplicate;
        delete [] values;
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
            bool isNull = false;

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
                    isNull = false;
                }
                else
                {
                    dataValue = (static_cast<FdoDataValue*>(literalExpression.p));
                    dataType = dataValue->GetDataType();
                    isNull = dataValue->IsNull();
                }
            }
            else
            {
                // Streamed based
                const FdoSmLpPropertyDefinition *propertyDefinition = classDefinition->RefProperties()->RefItem(name);

                const FdoSmLpDataPropertyDefinition* dataProp =
                        static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                dataType = dataProp->GetDataType();
                isNull = false;
            }

            FdoStringP bindValue;
            values[bindIndex-1].type = dataType;
            values[bindIndex-1].pos = bindIndex;
            values[bindIndex-1].null_ind = false;
            wcsncpy(values[bindIndex-1].propertyName, (const wchar_t *)name, GDBI_SCHEMA_ELEMENT_NAME_SIZE );

            if (isNull == false)
            {
                switch ( values[bindIndex-1].type )
                {
                    case FdoDataType_Boolean:
                        bindValue += ((dynamic_cast<FdoBooleanValue*>(dataValue))->GetBoolean())?L"1":L"0";
                        break;

                    case FdoDataType_Byte:
                        bindValue += mConnection->GetUtility()->Utf8ToUnicode( ut_itoa( (dynamic_cast<FdoByteValue*>(dataValue))->GetByte(), buffer ) );
                        break;

                    case FdoDataType_DateTime:
                        bindValue +=  mConnection->GetUtility()->Utf8ToUnicode( dynamic_cast<FdoRdbmsConnection*>(mFdoConnection)->FdoToDbiTime( (dynamic_cast<FdoDateTimeValue*>(dataValue))->GetDateTime() ) );
                        break;

                    case FdoDataType_Decimal:
                        sprintf(mTmpStringValue,"%.8g", (dynamic_cast<FdoDecimalValue*>(dataValue))->GetDecimal());
                        bindValue +=  mConnection->GetUtility()->Utf8ToUnicode( mTmpStringValue );
                        break;

                    case FdoDataType_Double:
                        sprintf(mTmpStringValue,"%.16g", (dynamic_cast<FdoDoubleValue*>(dataValue))->GetDouble());
                        bindValue +=  mConnection->GetUtility()->Utf8ToUnicode( mTmpStringValue );
                        break;

                    case FdoDataType_Int16:
                        bindValue +=  mConnection->GetUtility()->Utf8ToUnicode( ut_itoa( (dynamic_cast<FdoInt16Value*>(dataValue))->GetInt16(), buffer ) );
                        break;

                    case FdoDataType_Int32:
                        bindValue +=  mConnection->GetUtility()->Utf8ToUnicode( ut_itoa( (dynamic_cast<FdoInt32Value*>(dataValue))->GetInt32(),buffer ) );
                        break;

                    case FdoDataType_Int64:
#ifdef _WIN32
                        bindValue +=  mConnection->GetUtility()->Utf8ToUnicode( _i64toa( (dynamic_cast<FdoInt64Value*>(dataValue))->GetInt64(),mTmpStringValue, 10 ) );
#else

        sprintf(mTmpStringValue, "%lld", (dynamic_cast<FdoInt64Value*>(dataValue))->GetInt64());
         bindValue +=  mConnection->GetUtility()->Utf8ToUnicode(mTmpStringValue);
#endif
                        break;

                    case FdoDataType_Single:
                        sprintf(mTmpStringValue,"%.8g", (dynamic_cast<FdoSingleValue*>(dataValue))->GetSingle());
                        bindValue +=  mConnection->GetUtility()->Utf8ToUnicode( mTmpStringValue );
                        break;

                    case FdoDataType_String:
						{
							int index = bindIndex-1;
							FdoString* wvalue = (dynamic_cast<FdoStringValue*>(dataValue))->GetString();
							if( mConnection->GetGdbiCommands()->SupportsUnicode() )
							{
								values[index].value.strvalue = (wchar_t *)wvalue;
								values[index].valueNeedsFree = false;
								int size = (int) (wcslen(wvalue) + 1)*sizeof(wchar_t);
								statement->Bind( bindIndex++, size, (wchar_t *)values[index].value.strvalue, &values[index].null_ind );
							}
							else
							{
								int size = (int)wcslen(wvalue)*3;
								values[index].value.strvalue = new char[size+1];
								strncpy((char*)values[index].value.strvalue, mConnection->GetUtility()->UnicodeToUtf8(wvalue), size);
								((char*)values[index].value.strvalue)[size]='\0';
								values[index].valueNeedsFree = true;
								statement->Bind( bindIndex++, size, (char *)values[index].value.strvalue, &values[index].null_ind );

							}
							mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);						
						}
                        break;

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
										index = bindIndex-1;
										i++;
										values[index].len = 64;
										values[index].value.strvalue = new char[values[index].len];
										sprintf((char*)values[index].value.strvalue, "%.16g", x);
										values[index].type = FdoDataType_String;
										values[index].valueNeedsFree = true;
                                        mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);
										statement->Bind(bindIndex++, values[index].len, (const char*)values[index].value.strvalue, &values[index].null_ind );
									}
									if (NULL != columnY )
									{
										index = bindIndex-1;
										i++;
										values[index].len = 64;
										values[index].value.strvalue = new char[values[index].len];
										sprintf((char*)values[index].value.strvalue, "%.16g", y);
										values[index].type = FdoDataType_String;
										values[index].valueNeedsFree = true;
                                        mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);
										statement->Bind(bindIndex++, values[index].len, (const char*)values[index].value.strvalue, &values[index].null_ind );
									}
									if (NULL != columnZ )
									{
										index = bindIndex-1;
										i++;
										values[index].len = 64;
										values[index].value.strvalue = new char[values[index].len];
										sprintf((char*)values[index].value.strvalue, "%.16g", z);
										values[index].type = FdoDataType_String;
										values[index].valueNeedsFree = true;
                                        mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);
										statement->Bind(bindIndex++, values[index].len, (const char*)values[index].value.strvalue, &values[index].null_ind );
									}
								}
								break;
							}
                            // implicit else
                            FdoIGeometry        *geom = NULL;
                            if ( ba )
                            {
                                geom = mFdoConnection->TransformGeometry( 
                                    FdoPtr<FdoIGeometry>(gf->CreateGeometryFromFgf(ba)), 
                                    geomPropDef, 
                                    false 
                                );

                                // Validate the input geometry
                                mConnection->GetSchemaUtil()->CheckGeomPropOrdDimensionality( classDefinition, name, geom );
                                mConnection->GetSchemaUtil()->CheckGeomPropShapeType( classDefinition, name, geom );
                                mConnection->GetSchemaUtil()->CheckGeomPropValidity( classDefinition, name, geom );

                                mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);								
                            }
                            FdoIGeometry *old_geom = (FdoIGeometry*) values[index].value.strvalue;
                            FDO_SAFE_RELEASE( old_geom );
                            values[index].value.strvalue = (char*) geom;

                            statement->Bind(bindIndex++, (FdoIGeometry*)&values[index].value.strvalue, &values[index].null_ind );

							if( geom != NULL )
							{
								// Set SRID for the geometry column
								const FdoSmPhColumnP gColumn = ((FdoSmLpSimplePropertyDefinition*)geomPropDef)->GetColumn();
								FdoSmPhColumnGeomP geomCol = gColumn.p->SmartCast<FdoSmPhColumnGeom>();
								if (geomCol)
									statement->geom_srid_set(bindIndex-1, (long)geomCol->GetSRID());
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
                                        if (geomSiKeys != NULL && n < geomSiKeys->GetCount())
                                            wcharValue = geomSiKeys->GetString(n);

                                        if( mConnection->GetGdbiCommands()->SupportsUnicode() )
									    {
										    values[index].len = columnSi1->GetLength()+1;
										    values[index].value.strvalue = new wchar_t[values[index].len];
										    values[index].type = FdoDataType_String;
										    values[index].valueNeedsFree = true;
										    wcsncpy((wchar_t*)values[index].value.strvalue, wcharValue, values[index].len);
										    ((wchar_t*)values[index].value.strvalue)[values[index].len-1] = '\0';
										    statement->Bind(bindIndex++, values[index].len, (const wchar_t*)values[index].value.strvalue, &values[index].null_ind );
									    }
									    else
									    {
										    values[index].type = FdoDataType_String;
										    values[index].len = columnSi1->GetLength()+1;
										    values[index].value.strvalue = new char[values[index].len];
										    values[index].valueNeedsFree = true;
										    const char* charVal = mConnection->GetUtility()->UnicodeToUtf8( wcharValue );
										    strncpy((char*)values[index].value.strvalue, charVal, values[index].len);
										    ((char*)values[index].value.strvalue)[values[index].len-1] = '\0';
										    statement->Bind(bindIndex++, values[index].len, (const char*)values[index].value.strvalue, &values[index].null_ind );
									    }
                                        if (wcharValue[0] == L'\0')
								            mConnection->GetGdbiCommands()->set_null( &values[index].null_ind, 0, 0 );
                                        else
    									    mConnection->GetGdbiCommands()->set_nnull( &values[index].null_ind, 0, 0 );
								    }
								}
							}
                            break;
                        }

                    case FdoDataType_BLOB:
                        {
                            int index = bindIndex-1;
                            mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);

                            if ( streamReader != NULL )
                            {
                                // Oracle will return a LOB locator into .value.strvalue
                                values[index].reader = streamReader;
                                bindIndex++;
                            }
                            else
                            {
                                // Avoid copying the value by using directly the address of data
                                FdoBLOBValue * blob = static_cast<FdoBLOBValue*>(dataValue);
                                FdoByteArray * byteArr = blob->GetData();
                                values[index].value.strvalue = (char*) byteArr->GetData();

                                statement->Bind(bindIndex++, RDBI_BLOB, byteArr->GetCount(),
                                                       (char *)values[index].value.strvalue, &values[index].null_ind );
                            }
                            break;
                        }
                    default:
                        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_54, "Unhandled type: %1$d", dataValue->GetDataType() ));
                        break;
                }
		
				// Binding done above.
				if ( dataType == FdoRdbmsDataType_Geometry || dataType == FdoDataType_BLOB || dataType == FdoDataType_String)
					continue;
			}

            const char *val = "";
			if( ((const wchar_t*)bindValue)[0] != '\0' )
				val = mConnection->GetUtility()->UnicodeToUtf8((const wchar_t*)bindValue);
            int index = bindIndex-1;
            int size;
            if (isNull)
            {
                values[index].valueNeedsFree = true;
                values[index].value.strvalue = new char[2];
                val = " ";
                strcpy((char*)values[index].value.strvalue, val);
                mConnection->GetGdbiCommands()->set_null(&values[index].null_ind, 0,0);
                size = 2;
            }
            else
            {
                if (strlen(val) > 0)
                {
                    values[index].valueNeedsFree = true;
                    values[index].value.strvalue = new char[strlen(val) + 1];
                    strcpy( (char*)values[index].value.strvalue, val );
                    mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);
                    size = (int) strlen(val) + 1;
                }
                else
                {
                    values[index].valueNeedsFree = true;
                    values[index].value.strvalue = new char[2];
                    val = "";
                    strcpy((char*)values[index].value.strvalue, val);
                    mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);
                    size = 2;
                }
            }
            statement->Bind( bindIndex++, size, (char *)values[index].value.strvalue, &values[index].null_ind );
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

            FdoDataValue *dataValue = ((FdoDataValue*)literalExpression.p);

            // Values need to be converted to string as the bind variable are bound to a string
            const char *val = NULL;
            const wchar_t *wVal = NULL;
            FdoInt64   intVal = -1;
            switch ( dataValue->GetDataType() )
            {
                case FdoDataType_String:
                    if ( mConnection->GetGdbiCommands()->SupportsUnicode() )
                        wVal = (dynamic_cast<FdoStringValue*>(dataValue))->GetString();
                    else
                        val = mConnection->GetUtility()->UnicodeToUtf8((dynamic_cast<FdoStringValue*>(dataValue))->GetString());
                    break;

                case FdoDataType_Int64:
                    intVal = (dynamic_cast<FdoInt64Value*>(dataValue))->GetInt64();
                    break;

                case FdoDataType_Int32:
                    intVal = (dynamic_cast<FdoInt32Value*>(dataValue))->GetInt32();
                    break;

                case FdoDataType_Int16:
                    intVal = (dynamic_cast<FdoInt16Value*>(dataValue))->GetInt16();
                    break;

                default:
                    throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_54, "Unhandled type: %1$d", dataValue->GetDataType() ));
                    break;
            }
            if( val != NULL )
            {
                values[index].valueNeedsFree = true;
                values[index].value.strvalue = new char[strlen(val) + 1];
                strcpy( (char*)values[index].value.strvalue, val );
            }
            else if ( wVal != NULL ) 
            {
                values[index].valueNeedsFree = true;
                values[index].value.strvalue = (char*)(new wchar_t[wcslen(wVal) + 1]);
                wcscpy( (wchar_t*)values[index].value.strvalue, wVal );
            }
            else if( intVal != -1 )
            {
                values[index].valueNeedsFree = true;
                if ( mConnection->GetGdbiCommands()->SupportsUnicode() ) {
#ifdef _WIN32
                    swprintf((wchar_t*)mTmpStringValue,GDBI_MAXIMUM_STRING_SIZE,L"%I64d",intVal);
#else
                    swprintf((wchar_t*)mTmpStringValue,GDBI_MAXIMUM_STRING_SIZE,L"%lld",intVal);
#endif
                    values[index].value.strvalue = (char*)(new wchar_t[wcslen((wchar_t*)mTmpStringValue) + 1]);
                    wcscpy( (wchar_t*)values[index].value.strvalue, (wchar_t*)mTmpStringValue );
                }
                else {
#ifdef _WIN32
                    sprintf(mTmpStringValue,"%I64d",intVal);
#else
                    sprintf(mTmpStringValue,"%lld",intVal);
#endif
                    values[index].value.strvalue = new char[strlen(mTmpStringValue) + 1];
                    strcpy( (char*)values[index].value.strvalue, mTmpStringValue );
                }
            }

            values[index].pos = bindIndex;
            values[index].null_ind = false;
            mConnection->GetGdbiCommands()->set_nnull(&values[index].null_ind, 0,0);

            const wchar_t * name = propertyDefinition->GetName();
            wcsncpy(values[index].propertyName, (const wchar_t *)name, GDBI_SCHEMA_ELEMENT_NAME_SIZE );

            if ( mConnection->GetGdbiCommands()->SupportsUnicode() )
                statement->Bind(bindIndex++, (int)wcslen((wchar_t*)values[index].value.strvalue)+1, (wchar_t *)values[index].value.strvalue, &values[index].null_ind);
            else
                statement->Bind(bindIndex++, (int)strlen((char*)values[index].value.strvalue)+1, (char *)values[index].value.strvalue, &values[index].null_ind);
        }

		AdditionalBinds( statement, values, bindIndex );

        numberOfRows = statement->ExecuteNonQuery();

        // Stream Write ...
//        FdoRdbmsLobUtility::InsertStreamedLobs( mConnection, classDefinition, propValCollection, values, count );

        // Clean up
        for( i = 0; i < count; i++ )
        {
            if ( values[i].valueNeedsFree && 
				values[i].value.strvalue && 
				((values[i].type != FdoRdbmsDataType_Geometry) && 
				(values[i].type != FdoDataType_BLOB)))
                delete[] (char*)values[i].value.strvalue;
        }
        delete[] values;

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
        FdoCommandException *exp = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
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

void FdoRdbmsPvcUpdateHandler::AdditionalBinds( GdbiStatement *statement, FdoRdbmsPvcBindDef  *values, int & bindIndex  )
{
}
