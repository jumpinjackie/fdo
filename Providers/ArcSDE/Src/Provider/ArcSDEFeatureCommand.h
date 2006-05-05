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
 */

#ifndef ARCSDEFEATURECOMMAND_H
#define ARCSDEFEATURECOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Fdo.h"


template <class FDO_COMMAND> class ArcSDEFeatureCommand : public ArcSDECommand<FDO_COMMAND>
{
    friend class ArcSDEConnection;
    friend class ArcSDEFeatureReader;

protected:
    FdoPtr<FdoFilter> mFilter;         // the command filter
    FdoPtr<FdoIdentifier> mClassName;  // the name of the feature class
    FdoPtr<FdoPropertyValueCollection> mValues;  // the collection of values to insert or update

protected:

    // Constructs an instance of a command for the given connection.    
    ArcSDEFeatureCommand (FdoIConnection* connection) :
        ArcSDECommand<FDO_COMMAND>(connection)  // superclass constructor
    {
        // Initialize the property value collection:
        mValues = FdoPropertyValueCollection::Create ();
    }

    // Default destructor for a command.
    virtual ~ArcSDEFeatureCommand() 
    { 
    }

    virtual void Dispose()
    {
        delete this;
    }

    bool isSet (FdoPropertyValue* value)
    {
        FdoPtr<FdoValueExpression> expr = value->GetValue();
        return (expr != NULL);
    }

    // Set one or more values on a given ArcSDE stream:
    void assignValue (ArcSDEConnection* connection, SE_STREAM stream, CHAR* table, int index, FdoPropertyDefinition* definition, FdoPropertyValue* value);
    void assignValues (ArcSDEConnection *connection, SE_STREAM stream, CHAR* table, FdoPropertyDefinitionCollection *properties, FdoPropertyValueCollection *values, bool bAssignNulls = false);

    // Retrieve one value from an ArcSDE stream:
    FdoExpression* GetValueFromStream(SE_STREAM stream, int streamColumnIndex, FdoPropertyDefinition *fdoProperty);


    /// <summary> Handles read-only properties and default values in the given PropertyValue collection.</summary>
    void HandleReadOnlyAndDefaultValues(FdoPropertyValueCollection *propValues, bool bMakeNullsExplicit = false)
    {
        FdoPtr<FdoClassDefinition> classDef = this->mConnection->GetRequestedClassDefinition (mClassName);
        FdoPtr<FdoPropertyDefinitionCollection> propertyDefs = classDef->GetProperties();

        for (FdoInt32 i=0; i<propertyDefs->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> propertyDef = propertyDefs->GetItem(i);
            bool bIsSet = false;
            bool bHasDefaultValue = false;
            bool bIsIdentityProperty = false;
            bool bIsReadOnly = false;

            // Get the corresponding property value, if any:
            FdoPtr<FdoPropertyValue> propValue;
            propValue = GetItemNoThrow(propValues, propertyDef->GetName());
            bIsSet = (propValue!=NULL) && isSet(propValue);

            // Determine whether this property is read only:
            if (propertyDef->GetPropertyType() == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dataPropertyDef = static_cast<FdoDataPropertyDefinition*>(propertyDef.p);
                bIsReadOnly = dataPropertyDef->GetReadOnly();
            }

            // Handle this property:
            if (propertyDef->GetPropertyType() == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dataPropertyDef = static_cast<FdoDataPropertyDefinition*>(propertyDef.p);
                bIsIdentityProperty = IsIdentityProperty(classDef, dataPropertyDef->GetName());

                FdoString *defaultValue = dataPropertyDef->GetDefaultValue();
                bHasDefaultValue = (defaultValue != NULL) && (wcslen(defaultValue) > 0);


                if (bIsReadOnly)
                {
                    if (bIsSet)
                        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_CANNOT_SET_READONLY_PROPERTY, "Property '%1$ls' cannot be set because it is read-only.", propertyDef->GetName()));
                    if (!bHasDefaultValue && !bIsIdentityProperty)
                        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_MISSING_DEFAULT_VALUE, "Read-only property '%1$ls' requires a default value.", propertyDef->GetName()));
                    if (bHasDefaultValue && bIsIdentityProperty)
                        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_CANNOT_DEFAULT_READONLY_PROPERTY, "Read-only identity property '%1$ls' cannot have a default value.", propertyDef->GetName()));
                }

                if (!bIsSet && bHasDefaultValue)
                {
                    if (propValue != NULL)
                        propValue->SetValue(defaultValue);
                    else
                    {
                        propValue = FdoPropertyValue::Create();
                        propValue->SetName(dataPropertyDef->GetName());
                        propValue->SetValue(defaultValue);
                        propValues->Add(propValue);
                    }
                }
            }

            // Defect 630013 "Cannot execute an InsertCommand with an empty PropertyValuesCollection";
            // ArcSDE's SE_stream_insert_table() function fails if we specify 0 columns for update even though
            // this is a valid scenario if all columns are nullable.  To workaround this, on Insert command,
            // we need to make NULL values explicit (e.g. add a FdoPropertyValue of value NULL,
            // whenever the FdoPropertyValue is missing from the collection) so that we explicitly set
            // 1 or more columns on the call to SE_stream_insert_table().
            if (bMakeNullsExplicit && !bIsReadOnly && !bIsSet && (propValue==NULL))
            {
                propValue = FdoPropertyValue::Create();
                propValue->SetName(propertyDef->GetName());
                propValue->SetValue((FdoValueExpression*)NULL);
                propValues->Add(propValue);
            }
        }

        // Validate that all given property values have valid property names:
        for (FdoInt32 i=0; i<propValues->GetCount(); i++)
        {
            FdoPtr<FdoPropertyValue> propVal = propValues->GetItem(i);
            FdoPtr<FdoIdentifier> propValId = propVal->GetName();
            // NOTE: we can skip checking the base properties since this provider currently doesn't support inheritance
            FdoPtr<FdoPropertyDefinition> propertyDef = propertyDefs->FindItem(propValId->GetName());
            if (propertyDef == NULL)
                throw FdoException::Create(NlsMsgGet1(ARCSDE_PROPERTY_NOT_FOUND, "The property '%1$ls' was not found.", propValId->GetName()));
        }
    }

    /// <summary>Gets the FdoPropertyValueCollection that specifies the names and values
    /// of the properties for the instance to be inserted OR updated.</summary>
    /// <returns>Returns the list of properties and their values.</returns> 
    virtual FdoPropertyValueCollection* GetPropertyValues ()
    {
        FdoPtr<ArcSDEConnection> connection;

        // verify the connection
        connection = static_cast<ArcSDEConnection*>(this->GetConnection ());
        if (connection == NULL)
            throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

        return (FDO_SAFE_ADDREF(mValues.p));
    }

public:

    /// <summary>Gets the filter as a filter tree.</summary>
    /// <returns>Returns the filter object</returns> 
    virtual FdoFilter* GetFilter ();

    /// <summary>Sets the filter as a Filter tree.</summary>
    /// <param name="value">Input the filter object</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetFilter (FdoFilter* value);

    /// <summary>Sets the filter as a Filter tree.</summary>
    /// <param name="value">Input the filter expression string</param> 
    /// <returns>Returns nothing</returns>
    virtual void SetFilter (FdoString* value);

    /// <summary>Gets the name of the class to be operated upon as an identifier.</summary>
    /// <returns>Returns the class identifier.</returns> 
    virtual FdoIdentifier* GetFeatureClassName ();

    /// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
    /// <param name="value">Input the class identifier</param> 
    /// <returns>Returns nothing.</returns> 
    virtual void SetFeatureClassName (FdoIdentifier* value);

    /// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
    /// <param name="value">Input the class name</param> 
    /// <returns>Returns nothing</returns>
    virtual void SetFeatureClassName (FdoString* value);
};



/// <summary>Gets the filter as a filter tree.</summary>
/// <returns>Returns the filter object</returns> 
template <class FDO_COMMAND> 
FdoFilter* ArcSDEFeatureCommand<FDO_COMMAND>::GetFilter ()
{
    //TODO: should I return an empty filter here?
    return (FDO_SAFE_ADDREF(mFilter.p));
}

/// <summary>Sets the filter as a Filter tree.</summary>
/// <param name="value">Input the filter object</param> 
/// <returns>Returns nothing</returns> 
template <class FDO_COMMAND> 
void ArcSDEFeatureCommand<FDO_COMMAND>::SetFilter (FdoFilter* value)
{
    this->reset ();
    mFilter = FDO_SAFE_ADDREF(value);
}

/// <summary>Sets the filter as a Filter tree.</summary>
/// <param name="value">Input the filter expression string</param> 
/// <returns>Returns nothing</returns>
template <class FDO_COMMAND> 
void ArcSDEFeatureCommand<FDO_COMMAND>::SetFilter (FdoString* value)
{
    this->reset ();
    FdoPtr<FdoFilter> filter = FdoFilter::Parse (value);
    mFilter = FDO_SAFE_ADDREF(filter.p);
}



/// <summary>Gets the name of the class to be operated upon as an identifier.</summary>
/// <returns>Returns the class identifier.</returns> 
template <class FDO_COMMAND> 
FdoIdentifier* ArcSDEFeatureCommand<FDO_COMMAND>::GetFeatureClassName ()
{
    // addref the class name because we own it
    return (FDO_SAFE_ADDREF(mClassName.p));
}


/// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
/// <param name="value">Input the class identifier</param> 
/// <returns>Returns nothing.</returns> 
template <class FDO_COMMAND> 
void ArcSDEFeatureCommand<FDO_COMMAND>::SetFeatureClassName (FdoIdentifier* value)
{
    this->reset ();

    // Store the given feature class name:
    mClassName = FDO_SAFE_ADDREF(value);  // does automatic release
}


/// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
/// <param name="value">Input the class name</param> 
/// <returns>Returns nothing</returns>
template <class FDO_COMMAND> 
void ArcSDEFeatureCommand<FDO_COMMAND>::SetFeatureClassName (FdoString* value)
{
    FdoPtr<FdoIdentifier> class_name;

    if (value != NULL)
        class_name = FdoIdentifier::Create (value);
    else
        class_name = NULL;
    SetFeatureClassName (class_name);
}


template <class FDO_COMMAND> 
void ArcSDEFeatureCommand<FDO_COMMAND>::assignValue (ArcSDEConnection* connection, SE_STREAM stream, CHAR* table, int columnIndex, FdoPropertyDefinition* definition, FdoPropertyValue* value)
{
    bool null = false;
    FdoDataType type = (FdoDataType)0L;
    LONG ret = 0L;

    // do this nonsense until the FDO useability review:
    null = false;
    // for some reason the dynamic cast operator isn't handled by the 'smart' pointer
    // and the reference count goes up unless direct pointers are used
    {
        FdoPtr<FdoValueExpression> expression = value->GetValue ();
        FdoDataValue* v1 = dynamic_cast<FdoDataValue*>(expression.p);
        if (v1 != NULL)
            null = v1->IsNull ();
        else
        {
            FdoGeometryValue* v2 = dynamic_cast<FdoGeometryValue*>(expression.p);
            if (v2 != NULL)
                null = v2->IsNull ();
            else
                null = true;
        }

        // If a stream reader is present, this value is considered NOT null:
        FdoPtr<FdoIStreamReader> streamReader = value->GetStreamReader();
        if (streamReader != NULL)
            null = false;
    }


    switch (definition->GetPropertyType ())
    {
        case FdoPropertyType_DataProperty:
            {
                FdoDataPropertyDefinition* data_property = static_cast<FdoDataPropertyDefinition*>(definition);
                type = data_property->GetDataType ();
                switch (type)
                {
                    case FdoDataType_Int16: // 2-byte integer
                        {
                            FdoInt16Value* _int = NULL;
                            SHORT __int;

                            if (null)
                                ret = SE_stream_set_smallint (stream, columnIndex, NULL);
                            else
                            {
                                FdoPtr<FdoValueExpression> valueExpr = value->GetValue ();
                                _int = dynamic_cast<FdoInt16Value*>(valueExpr.p);
                                if (_int == NULL)
                                    throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"FdoInt16", definition->GetName()));

                                __int = _int->GetInt16 ();
                                ret = SE_stream_set_smallint (stream, columnIndex, &__int);
                            }
                        }
                        break;
                    case FdoDataType_Int32: // 4-byte integer
                        {
                            FdoInt32Value* _int = NULL;
                            LONG __int;

                            if (null)
                                ret = SE_stream_set_integer (stream, columnIndex, NULL);
                            else
                            {
                                FdoPtr<FdoValueExpression> valueExpr = value->GetValue ();
                                _int = dynamic_cast<FdoInt32Value*>(valueExpr.p);
                                if (_int == NULL)
                                    throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"FdoInt32", definition->GetName()));

                                __int = _int->GetInt32 ();
                                ret = SE_stream_set_integer (stream, columnIndex, &__int);
                            }
                        }
                        break;
                    case FdoDataType_Single: // 4-byte float
                        {
                            FdoSingleValue* _float = NULL;
                            FLOAT __float;

                            if (null)
                                ret = SE_stream_set_float (stream, columnIndex, NULL);
                            else
                            {
                                FdoPtr<FdoValueExpression> valueExpr = value->GetValue ();
                                _float = dynamic_cast<FdoSingleValue*>(valueExpr.p);
                                if (_float == NULL)
                                    throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"FdoSingle", definition->GetName()));

                                __float = _float->GetSingle ();
                                ret = SE_stream_set_float (stream, columnIndex, &__float);
                            }
                        }
                        break;
                    case FdoDataType_Double: // 8-byte float
                        {
                            FdoDoubleValue* _double = NULL;
                            double __double;

                            if (null)
                                ret = SE_stream_set_double (stream, columnIndex, NULL);
                            else
                            {
                                FdoPtr<FdoValueExpression> valueExpr = value->GetValue ();
                                _double = dynamic_cast<FdoDoubleValue*>(valueExpr.p);
                                if (_double == NULL)
                                    throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"FdoDouble", definition->GetName()));

                                __double = _double->GetDouble ();
                                ret = SE_stream_set_double (stream, columnIndex, &__double);
                            }
                        }
                        break;
                    case FdoDataType_String: // Null term. Character array
                        {
                            FdoStringValue* _string = NULL;
                            CHAR* __string;

                            if (null)
                                ret = SE_stream_set_string (stream, columnIndex, NULL);
                            else
                            {
                                FdoPtr<FdoValueExpression> valueExpr = value->GetValue ();
                                _string = dynamic_cast<FdoStringValue*>(valueExpr.p);
                                if (_string == NULL)
                                    throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"FdoString", definition->GetName()));

                                wide_to_multibyte (__string, (wchar_t*)_string->GetString ());
                                ret = SE_stream_set_string (stream, columnIndex, __string);
                            }
                        }
                        break;
                    case FdoDataType_BLOB: // Variable length data
                        {
                            SE_BLOB_INFO __blob;

                            if (null)
                                ret = SE_stream_set_blob (stream, columnIndex, NULL);
                            else
                            {
                                FdoPtr<FdoIStreamReader> streamReader = value->GetStreamReader ();
                                FdoBLOBStreamReader* blobStreamReader = dynamic_cast<FdoBLOBStreamReader*>(streamReader.p);
                                if (blobStreamReader != NULL)
                                {
                                    FdoInt64 blobLength = blobStreamReader->GetLength();
                                    __blob.blob_buffer = new char[(size_t)blobLength];
                                    __blob.blob_length = (long)blobLength;
                                    FdoInt32 bytesRead = blobStreamReader->ReadNext((FdoByte*)__blob.blob_buffer);
                                    if (bytesRead != blobLength)
                                        throw FdoException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));
                                }
                                else
                                {
                                    FdoPtr<FdoByteArray> bytes;
                                    FdoPtr<FdoValueExpression> valueExpr = value->GetValue();
                                    FdoBLOBValue* blobValue = dynamic_cast<FdoBLOBValue*>(valueExpr.p);
                                    if (blobValue == NULL)
                                        throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"FdoBLOB", definition->GetName()));

                                    bytes = blobValue->GetData();
                                    __blob.blob_buffer = (char*)bytes->GetData ();
                                    __blob.blob_length = bytes->GetCount ();
                                    if (NULL == __blob.blob_buffer)
                                        __blob.blob_length = 0;
                                }

                                // TODO: use use SE_stream_bind_input_column() isntead of SE_stream_set_blob()
                                //       to improve performance (i.e. avoid arcsde duplicating the entire blob!).
                                ret = SE_stream_set_blob (stream, columnIndex, &__blob);

                                // Clean up:
                                if (blobStreamReader != NULL)
                                    SE_blob_free(&__blob);
                            }
                        }
                        break;
                    case FdoDataType_DateTime: // Struct tm date
                        {
                            FdoDateTimeValue* _date = NULL;
                            FdoDateTime fdoDateTime;
                            struct tm sdeDateTime;

                            if (null)
                                ret = SE_stream_set_date (stream, columnIndex, NULL);
                            else
                            {
                                FdoPtr<FdoValueExpression> valueExpr = value->GetValue();
                                _date = dynamic_cast<FdoDateTimeValue*>(valueExpr.p);
                                if (_date == NULL)
                                    throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"FdoDateTime", definition->GetName()));

                                fdoDateTime = _date->GetDateTime ();

                                sdeDateTime = FdoDateTime2SdeDateTime(fdoDateTime);

                                ret = SE_stream_set_date (stream, columnIndex, &sdeDateTime);
                            }
                        }
                        break;
                    //
                    // these aren't supported YET:
                    //
                    case FdoDataType_Decimal:
                        throw FdoException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_DECIMAL, "The Decimal data type is not supported by ArcSDE."));
                        break;
                    case FdoDataType_Boolean:
                        throw FdoException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_BOOLEAN, "The Boolean data type is not supported by ArcSDE."));
                        break;
                    case FdoDataType_Int64:
                        throw FdoException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_INT64, "The Int64 data type is not supported by ArcSDE."));
                        break;
                    case FdoDataType_Byte:
                        throw FdoException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_BYTE, "The Byte data type is not supported by ArcSDE."));
                        break;

                    //
                    // these will not be supported (at least not in R1):
                    //
                    case FdoDataType_CLOB:
                    default:
                        throw FdoException::Create (NlsMsgGet1(ARCSDE_DATATYPE_UNHANDLED, "The FDO DataType %1$d is unsupported.", (int)type));
                        break;
                }
            }
            break;

        case FdoPropertyType_ObjectProperty:
            throw FdoException::Create (NlsMsgGet(ARCSDE_OBJECT_PROPERTIES_NOT_SUPPORTED, "Object properties are not supported."));
            break;

        case FdoPropertyType_GeometricProperty:
            {
                SE_COLUMN_DEF column;
                SE_COORDREF coordref;
                SE_SHAPE shape;

                if (null)
                    ret = SE_stream_set_shape (stream, columnIndex, NULL);
                else
                {
                    FdoPtr<FdoValueExpression> valueExpr = value->GetValue ();
                    FdoGeometryValue* geometry = dynamic_cast<FdoGeometryValue*>(valueExpr.p);
                    if (geometry == NULL)
                        throw FdoException::Create(NlsMsgGet2(ARCSDE_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"Geometry", definition->GetName()));

                    ret = SE_stream_describe_column (stream, columnIndex, &column);
                    handle_sde_err<FdoException>(stream, ret, __FILE__, __LINE__, ARCSDE_STREAM_GET_COLUMN_FAILED, "Failed to retrieve column information from the stream.");

                    ret = GetCoordRefFromColumn (connection->GetConnection(), table, column.column_name, coordref);
                    handle_sde_err<FdoException>(stream, ret, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");

                    FdoPtr<FdoByteArray> fgf = geometry->GetGeometry ();
                    try
                    {
                        convert_fgf_to_sde_shape (connection, fgf, coordref, shape);
                        SE_coordref_free (coordref);
                    }
                    catch (FdoException *e)
                    {
                        SE_coordref_free (coordref);
                        SE_shape_free (shape);
                        throw e;
                    }

                    ret = SE_stream_set_shape (stream, columnIndex, shape);
                    // ret is handled after 'switch' block

                    SE_shape_free (shape);
                }
            }
            break;
    }

    wchar_t* wTableName = NULL;
    multibyte_to_wide(wTableName, table);
    handle_sde_err<FdoCommandException>(stream, ret, __FILE__, __LINE__, ARCSDE_COULDNT_SET_VALUE,
        "Failed to set value for column %1$d (property %2$ls) on table %3$ls (class %4$ls).", columnIndex, definition->GetName (), wTableName, FdoPtr<FdoSchemaElement>(definition->GetParent ())->GetName ());
}



template <class FDO_COMMAND> 
void ArcSDEFeatureCommand<FDO_COMMAND>::assignValues (ArcSDEConnection *connection, SE_STREAM stream, CHAR* table, FdoPropertyDefinitionCollection *properties, FdoPropertyValueCollection *values, bool bAssignNulls)
{
    // ToDo: use input bind variables
    int j = 0;
    for (int i = 0; i < values->GetCount (); i++)
    {
        FdoPtr<FdoPropertyValue> value = values->GetItem (i);
        FdoPtr<FdoIdentifier> propertyId = value->GetName();
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem (propertyId->GetName());
        if (isSet (value) || bAssignNulls)
        {
            assignValue (connection, stream, table, j + 1, property, value);
            j++;
        }
    }
}


template <class FDO_COMMAND> 
FdoExpression* ArcSDEFeatureCommand<FDO_COMMAND>::GetValueFromStream(SE_STREAM stream, int streamColumnIndex, FdoPropertyDefinition *fdoProperty)
{
    // TODO: handle geometry properties:
    if (fdoProperty->GetPropertyType() != FdoPropertyType_DataProperty)
        throw FdoException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));

    FdoPtr<FdoExpression> expr;
    LONG result = SE_SUCCESS;
    FdoDataPropertyDefinition* fdoDataProperty = dynamic_cast<FdoDataPropertyDefinition*>(fdoProperty);
    LONG sdeType = FdoType2SDEType(fdoDataProperty->GetDataType());

    // Get the fdoProperty value for this row:
    switch (sdeType)
    {
        case SE_INT16_TYPE:
            SHORT idInt16;
            result = SE_stream_get_smallint(stream, streamColumnIndex, &idInt16);
            expr = FdoInt16Value::Create(idInt16);
        break;

        case SE_INT32_TYPE:
            LONG  idInt32;
            result = SE_stream_get_integer(stream, streamColumnIndex, &idInt32);
            expr = FdoInt32Value::Create(idInt32);
        break;

        case SE_FLOAT32_TYPE:
            FLOAT idFloat32;
            result = SE_stream_get_float(stream, streamColumnIndex, &idFloat32);
            expr = FdoSingleValue::Create(idFloat32);
        break;

        case SE_FLOAT64_TYPE:
            LFLOAT idFloat64;
            result = SE_stream_get_double(stream, streamColumnIndex, &idFloat64);
            expr = FdoDoubleValue::Create(idFloat64);
        break;

        case SE_STRING_TYPE:
        {
            CHAR  *idString = (CHAR*)alloca((fdoDataProperty->GetLength()+1) * sizeof(CHAR));
            result = SE_stream_get_string(stream, streamColumnIndex, idString);
            wchar_t *wIdString = NULL;
            multibyte_to_wide(wIdString, idString);
            expr = FdoStringValue::Create(wIdString);
        }
        break;

        case SE_DATE_TYPE:
            struct tm idDateTime;
            result = SE_stream_get_date(stream, streamColumnIndex, &idDateTime);
            expr = FdoDateTimeValue::Create( SdeDateTime2FdoDateTime(idDateTime) );
        break;

        default:
            result = SE_FAILURE;
        break;
    }
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_UPDATE_FAILED, "Update table failed.");

    return FDO_SAFE_ADDREF(expr.p);
}


#endif // ARCSDEFEATURECOMMAND_H

