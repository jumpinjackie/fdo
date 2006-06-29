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

#include "Pch.h"
#include "ArcSDETests.h"


ArcSDETests::ArcSDETests(void)
{
}

ArcSDETests::~ArcSDETests(void)
{
}

FdoIConnection* ArcSDETests::GetConnection ()
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    return (manager->CreateConnection (L"OSGeo.ArcSDE.3.2"));
}

void ArcSDETests::fail (FdoException* ge)
{
    wchar_t message[5000];
    char* multibyte;
	FdoPtr<FdoException> cause;

    wcscpy(message, (wchar_t*)ge->GetExceptionMessage ());
	cause = ge->GetCause();
	while (cause != NULL)
	{
		wcscat(message, L" [");
		wcscat(message, cause->GetExceptionMessage());
		wcscat(message, L"]");
		cause = cause->GetCause();
	}
    wide_to_multibyte (multibyte, message);
    if (NULL == multibyte)
        multibyte = "Exception message could not be converted.";
    ge->Release ();
    CPPUNIT_FAIL (multibyte);
}


void ArcSDETests::CleanUpClass(FdoIConnection *connection, const wchar_t* schema_name, const wchar_t* class_name, bool bDeleteRowsOnly)
{
    FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (class_name);
	FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
	if (NULL!=schema_name)
		descSchema->SetSchemaName(schema_name);
	FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();

	bool bFound = false;
	for (FdoInt32 iSchemaIndex = 0; iSchemaIndex<schemas->GetCount(); iSchemaIndex++)
	{
		FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(iSchemaIndex);
		if ((NULL==schema_name) || (0==wcscmp(schema->GetName(), schema_name)))
		{
			FdoPtr<FdoClassCollection> classes = schema->GetClasses();
			for (FdoInt32 iClassIndex = 0; iClassIndex<classes->GetCount(); iClassIndex++)
			{
				FdoPtr<FdoClassDefinition> classDef = classes->GetItem(iClassIndex);
				if (0==FdoCommonOSUtil::wcsicmp(classDef->GetName(), identifier->GetName ()))
				{
					bFound = true;

                    if (bDeleteRowsOnly)
                    {
                        // delete the rows of this class, but not the class itself:
                        FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)connection->CreateCommand (FdoCommandType_Delete);
                        deleteCmd->SetFeatureClassName(classDef->GetQualifiedName());
                        deleteCmd->SetFilter(L"1=1");
                        deleteCmd->Execute();
                    }
                    else
                    {
                        // delete the entire class:
					    classDef->Delete();
					    FdoPtr<FdoIApplySchema> applySchema = (FdoIApplySchema*)connection->CreateCommand(FdoCommandType_ApplySchema);
					    applySchema->SetFeatureSchema(schema);
					    applySchema->Execute();
                    }

					break;
				}
			}
		}

		if (bFound)
			break;
	}

/*  // The following code can lead to issues if two classes have same name in different schemas:
	// recurse with default schema, if class not found:
	if (!bFound && (NULL!=schema_name) && (wcslen(schema_name)>0))
		CleanUpClass(connection, NULL, class_name, bDeleteRowsOnly);
*/
}


FdoFeatureSchema* ArcSDETests::GetDefaultSchema(FdoIConnection* connection)
{
    FdoString* wUser;
    FdoString* wDatabase;
    FdoStringP wSchemaName;

    FdoPtr<FdoIConnectionInfo> info = (FdoIConnectionInfo*)connection->GetConnectionInfo ();
    FdoPtr<FdoIConnectionPropertyDictionary> dictionary = info->GetConnectionProperties ();

    wUser = dictionary->GetProperty (CONNECTIONPROPERTY_USERNAME);
    wDatabase = dictionary->GetProperty (CONNECTIONPROPERTY_DATASTORE);

    // Determine FDO schema name based on usern ame and database name:
    if ((wDatabase == NULL) || (wcslen(wDatabase)==0) || (0==FdoCommonOSUtil::wcsicmp(wDatabase,L"Default Datastore")))
    {
        wSchemaName = wUser;
        wSchemaName = wSchemaName.Upper();
    }
    else
    {
        wSchemaName  = wDatabase;
        wSchemaName += L"_";
        wSchemaName += wUser;
        wSchemaName = wSchemaName.Upper();
    }

    FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)connection->CreateCommand (FdoCommandType_DescribeSchema);
    describe->SetSchemaName (wSchemaName);
    FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

    return (schemas->GetItem (wSchemaName));
}

void ArcSDETests::CreateSchema (FdoIConnection* connection, UnitTestClass* definition)
{
    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)connection->CreateCommand (FdoCommandType_ApplySchema);
    FdoPtr<FdoFeatureSchema> schema = GetDefaultSchema (connection);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

    FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (definition->mName);
    FdoPtr<FdoClass> cls = FdoClass::Create (identifier->GetName (), definition->mDescription);
    FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> identities = cls->GetIdentityProperties ();
    for (size_t i = 0; i < definition->mProperties.size (); i++)
    {
        FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (definition->mProperties[i]->mPropertyName, definition->mProperties[i]->mPropertyDescription);
        property->SetDataType (definition->mProperties[i]->mPropertyType);
        property->SetLength (definition->mProperties[i]->mPropertyLength);
        property->SetPrecision (definition->mProperties[i]->mPropertyPrecision);
        property->SetScale (definition->mProperties[i]->mPropertyScale);
        property->SetNullable (definition->mProperties[i]->mPropertyNullable);
        property->SetIsAutoGenerated (definition->mProperties[i]->mPropertyAutoGenerated);
        property->SetReadOnly (definition->mProperties[i]->mPropertyReadOnly);
        properties->Add (property);
        if (definition->mProperties[i]->mPropertyAutoGenerated)
            identities->Add (property);
    }

    // set class capabilities:
    SetClassCapabilities(cls, definition->mSupportsLocking, definition->mSupportsLT);

    // submit the new schema
    classes->Add (cls);
    apply->SetFeatureSchema (schema);
    apply->Execute ();
}

void ArcSDETests::DeleteSchema (FdoIConnection* connection, UnitTestClass* definition)
{
    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)connection->CreateCommand (FdoCommandType_ApplySchema);
    FdoPtr<FdoFeatureSchema> schema = GetDefaultSchema(connection);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

    FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (definition->mName);
    FdoPtr<FdoClass> cls = (FdoClass*)classes->GetItem (identifier->GetName ());
    cls->Delete ();
    apply->SetFeatureSchema (schema);
    apply->Execute ();
}

void ArcSDETests::SetClassCapabilities(FdoClassDefinition *cls, bool bSupportLocking, bool bSupportLongTransactions)
{
    FdoPtr<FdoClassCapabilities> capabilities = FdoClassCapabilities::Create(*cls);
    capabilities->SetSupportsLocking(bSupportLocking);
    capabilities->SetSupportsLongTransactions(bSupportLongTransactions);
    cls->SetCapabilities(capabilities);
}

int ArcSDETests::Insert (FdoIConnection* connection, UnitTestClass* definition, size_t index)
{
    wchar_t* id;
    wchar_t* name;
    FdoPtr<FdoValueExpression> expression;
    FdoPtr<FdoPropertyValue> value;
    int ret;

    ret = -1;

    FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (definition->mName);
    FdoPtr<FdoIInsert> insert = (FdoIInsert*)connection->CreateCommand (FdoCommandType_Insert);
    insert->SetFeatureClassName (identifier->GetText ());
    FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
    for (size_t i = 0; i < definition->mProperties.size (); i++)
    {
        if (definition->mProperties[i]->mPropertyAutoGenerated)
            id = definition->mProperties[i]->mPropertyName;
        else
        {
            name = definition->mProperties[i]->mPropertyName;
            if (definition->mProperties[i]->mPropertyData.size () > index)
                expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (definition->mProperties[i]->mPropertyData[index], definition->mProperties[i]->mPropertyType);
            else
                expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (L"null", definition->mProperties[i]->mPropertyType);
            value = FdoPropertyValue::Create (name, expression);
            values->Add (value);
        }
    }
    FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
    while (reader->ReadNext ())
        if (-1 != ret)
            CPPUNIT_FAIL ("too many features");
        else
            ret = reader->GetInt32 (id);
    reader->Close ();

    return (ret);
}

void ArcSDETests::Delete (FdoIConnection* connection, UnitTestClass* definition, int id)
{
    wchar_t buffer[1024];
    FdoPtr<FdoFilter> filter;
    FdoPtr<FdoIDelete> del;

    FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (definition->mName);
    wcscpy (buffer, definition->mProperties[definition->mIdIndex]->mPropertyName);
    wcscat (buffer, L" = ");
    FdoCommonOSUtil::itow (id, buffer + wcslen (buffer), ELEMENTS(buffer) - wcslen(buffer));
    filter = FdoFilter::Parse (buffer);
    del = (FdoIDelete*)connection->CreateCommand (FdoCommandType_Delete);
    del->SetFeatureClassName (identifier->GetText ());
    del->SetFilter (filter);
    del->Execute ();
}

void ArcSDETests::Update (FdoIConnection* connection, UnitTestClass* definition, std::vector<int*> *map)
{
    wchar_t buffer[1024];
    FdoPtr<FdoFilter> filter;
    FdoPtr<FdoIUpdate> update;
    FdoPtr<FdoPropertyValueCollection> values;
    wchar_t* name;
    FdoPtr<FdoValueExpression> expression;
    FdoPtr<FdoPropertyValue> value;
    int count;

    FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (definition->mName);
    for (size_t i = 0; i < map->size (); i++)
    {
        wcscpy (buffer, definition->mProperties[definition->mIdIndex]->mPropertyName);
        wcscat (buffer, L" = ");
        FdoCommonOSUtil::itow ((*map)[i][1], buffer + wcslen (buffer), ELEMENTS(buffer) - wcslen(buffer));
        filter = FdoFilter::Parse (buffer);
        update = (FdoIUpdate*)connection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (identifier->GetText ());
        update->SetFilter (filter);
        values = update->GetPropertyValues ();
        for (size_t j = 0; j < definition->mProperties.size (); j++)
            if (definition->mIdIndex != j)
            {
                name = definition->mProperties[j]->mPropertyName;
                expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (definition->mProperties[j]->mPropertyData[(*map)[i][0]], definition->mProperties[j]->mPropertyType);
                value = FdoPropertyValue::Create (name, expression);
                values->Add (value);
            }
        count = update->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("update failed", 0 != count);
    }
}

void ArcSDETests::Expect (FdoIConnection* connection, UnitTestClass* definition, std::vector<int*> *map)
{
    FdoPtr<FdoISelect> select;
    FdoPtr<FdoIFeatureReader> reader;
    FdoPtr<FdoExpression> value;
    int identity;
    bool found;
    int got; // up to 32 objects
    bool expect_throw;
    
    FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (definition->mName);
    select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (identifier->GetText ());
    reader = select->Execute ();
    got = 0;
    while (reader->ReadNext ())
    {
        identity = reader->GetInt32 (definition->mProperties[definition->mIdIndex]->mPropertyName);
        found = false;
        for (size_t i = 0; i < map->size (); i++)
            if ((*map)[i][1] == identity)
            {
                for (size_t j = 0; j < definition->mProperties.size (); j++)
                    if (definition->mIdIndex != j)
                    {
                        size_t index = (*map)[i][0];
                        if (definition->mProperties[j]->mPropertyData.size () > index)
                            value = ArcSDETests::ParseByDataType (definition->mProperties[j]->mPropertyData[index], definition->mProperties[j]->mPropertyType);
                        else
                            value = ArcSDETests::ParseByDataType (L"null", definition->mProperties[j]->mPropertyType);
                        FdoPtr<FdoDataValue> data = dynamic_cast<FdoDataValue*>(FDO_SAFE_ADDREF(value.p));
                        if (data != NULL && data->IsNull ())
                        {
                            CPPUNIT_ASSERT_MESSAGE ("expecting null", reader->IsNull (definition->mProperties[j]->mPropertyName));
                            expect_throw = true;
                        }
                        else
                            expect_throw = false;
                        try
                        {
                            switch (definition->mProperties[j]->mPropertyType)
                            {
                                case FdoDataType_Int16:
                                    {
                                        FdoInt16 right = reader->GetInt16 (definition->mProperties[j]->mPropertyName);
                                        if (expect_throw)
                                            CPPUNIT_FAIL ("Expected an exception");
                                        expect_throw = false;
                                        FdoInt16 left = ((FdoInt16Value*)value.p)->GetInt16 ();
                                        if (left != right)
                                        {
                                            char buffer[1024];
                                            sprintf (buffer, "incorrect int16 value (expected %d, got %d)", left, right);
                                            CPPUNIT_FAIL (buffer);
                                        }
                                    }
                                    break;
                                case FdoDataType_Int32:
                                    {
                                        FdoInt32 right = reader->GetInt32 (definition->mProperties[j]->mPropertyName);
                                        if (expect_throw)
                                            CPPUNIT_FAIL ("Expected an exception");
                                        expect_throw = false;
                                        FdoInt32 left = ((FdoInt32Value*)value.p)->GetInt32 ();
                                        if (left != right)
                                        {
                                            char buffer[1024];
                                            sprintf (buffer, "incorrect int32 value (expected %d, got %d)", left, right);
                                            CPPUNIT_FAIL (buffer);
                                        }
                                    }
                                    break;
                                case FdoDataType_String:
                                    {
                                        const wchar_t* right = reader->GetString (definition->mProperties[j]->mPropertyName);
                                        if (expect_throw)
                                            CPPUNIT_FAIL ("Expected an exception");
                                        expect_throw = false;
                                        const wchar_t* left = ((FdoStringValue*)value.p)->GetString ();
                                        if (0 != wcscmp (left, right))
                                        {
                                            char* buffer;
                                            buffer = (char*)alloca (1024 + wcslen (left) + wcslen (right));
                                            sprintf (buffer, "incorrect string value (expected \"%ls\", got \"%ls\")", left, right);
                                            CPPUNIT_FAIL (buffer);
                                        }
                                    }
                                    break;
                                case FdoDataType_Double:
                                    {
                                        double right = reader->GetDouble (definition->mProperties[j]->mPropertyName);
                                        if (expect_throw)
                                            CPPUNIT_FAIL ("Expected an exception");
                                        expect_throw = false;
                                        double left = ((FdoDoubleValue*)value.p)->GetDouble ();
                                        if (left != right)
                                        {
                                            char buffer[1024];
                                            sprintf (buffer, "incorrect double value (expected %g, got %g)", left, right);
                                            CPPUNIT_FAIL (buffer);
                                        }
                                    }
                                    break;
                                case FdoDataType_Single:
                                    {
                                        float right = reader->GetSingle (definition->mProperties[j]->mPropertyName);
                                        if (expect_throw)
                                            CPPUNIT_FAIL ("Expected an exception");
                                        expect_throw = false;
                                        float left = ((FdoSingleValue*)value.p)->GetSingle ();
                                        if (left != right)
                                        {
                                            char buffer[1024];
                                            sprintf (buffer, "incorrect single value (expected %g, got %g)", left, right);
                                            CPPUNIT_FAIL (buffer);
                                        }
                                    }
                                    break;
                // ToDo:
                //FdoDataType_DateTime,
                //FdoDataType_BLOB,
                                default:
                                    CPPUNIT_FAIL ("unknown data type");
                            }
                        }
                        catch (FdoException* ge)
                        {
                            if (!expect_throw)
                                throw ge;
                            else
                                expect_throw = false;
                            ge->Release ();
                        }
                }
                got |= (1 << i);
                found = true;
            }
        CPPUNIT_ASSERT_MESSAGE ("unknown id", found);
    }
    reader->Close ();
    for (size_t i = 0; i < map->size (); i++)
        if (0 == (got & (1 << i)))
            CPPUNIT_FAIL ("missing object");
}

wchar_t* ArcSDETests::GetDataTypeString (FdoDataType type)
{
    wchar_t* ret;

    switch (type)
    {
        case FdoDataType_Boolean: // Represents a Boolean value of true or false.
            ret = L"FdoDataType_Boolean";
            break;
        case FdoDataType_Byte: // Represents unsigned 8-bit integers with values between 0 and 255.
            ret = L"FdoDataType_Byte";
            break;
        case FdoDataType_DateTime: // Represents a date and time value.
            ret = L"FdoDataType_DateTime";
            break;
        case FdoDataType_Decimal: // Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28 with 28-29 significant digits.
            ret = L"FdoDataType_Decimal";
            break;
        case FdoDataType_Double: // Represents a floating point value ranging from approximately 5.0 x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
            ret = L"FdoDataType_Double";
            break;
        case FdoDataType_Int16: // Represents signed 16-bit integers with values between -32768 and 32767.
            ret = L"FdoDataType_Int16";
            break;
        case FdoDataType_Int32: // Represents signed 32-bit integers with values between -2147483648 and 2147483647.
            ret = L"FdoDataType_Int32";
            break;
        case FdoDataType_Int64: // Represents signed 64-bit integers with values between -9223372036854775808 and 9223372036854775807.
            ret = L"FdoDataType_Int64";
            break;
        case FdoDataType_Single: // Represents floating point values ranging from approximately 1.5 x 10^-45 to 3.4 x 10^38 with a precision of 7 digits.
            ret = L"FdoDataType_Single";
            break;
        case FdoDataType_String: // Represents a Unicode character strings.
            ret = L"FdoDataType_String";
            break;
        case FdoDataType_BLOB: // Represents a binary large object stored as a collection of bytes.
            ret = L"FdoDataType_BLOB";
            break;
        case FdoDataType_CLOB: // Represents a character large object stored as a collection of  characters.
            ret = L"FdoDataType_CLOB";
            break;
        default:
            ret = L"Unknown return type";
            break;
    }

    return (ret);
}


FdoExpression* ArcSDETests::ParseByDataType(const wchar_t* data, FdoDataType dataType)
{
    FdoPtr<FdoExpression> expr;
    bool bIsNull = false;

    // NOTE: blob parsing doesn't work yet (ever?) in FDO:
    if (dataType != FdoDataType_BLOB)
    {
        expr = FdoExpression::Parse(data);

        // NOTE: The FDO parser handles nulls strangely (it always returns a Boolean type with IsNull set to true):
        FdoBooleanValue* nullChecker = dynamic_cast<FdoBooleanValue*>(expr.p);
        if (nullChecker != NULL)
            bIsNull = nullChecker->IsNull();
    }


    switch (dataType)
    {
        case FdoDataType_Boolean:
        {
            if (bIsNull)
            {
                FdoBooleanValue *val = FdoBooleanValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoBooleanValue* value = dynamic_cast<FdoBooleanValue*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
            }
        }
        break;

        case FdoDataType_Byte:
        {
            if (bIsNull)
            {
                FdoByteValue *val = FdoByteValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoInt32Value* value = dynamic_cast<FdoInt32Value*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
                expr = FdoByteValue::Create((FdoByte)value->GetInt32());
            }
        }
        break;

        case FdoDataType_Int16:
        {
            if (bIsNull)
            {
                FdoInt16Value *val = FdoInt16Value::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoInt32Value* value = dynamic_cast<FdoInt32Value*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
                expr = FdoInt16Value::Create((FdoInt16)value->GetInt32());
            }
        }
        break;

        case FdoDataType_Int32:
        {
            if (bIsNull)
            {
                FdoInt32Value *val = FdoInt32Value::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoInt32Value* value = dynamic_cast<FdoInt32Value*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
            }
        }
        break;

        case FdoDataType_Int64:
        {
            if (bIsNull)
            {
                FdoInt64Value *val = FdoInt64Value::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                //TODO: validate this code; I'm not sure if the parse handles int64 properly (returned as a double??)
                FdoInt32Value* value = dynamic_cast<FdoInt32Value*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
                expr = FdoInt64Value::Create((FdoInt64)value->GetInt32());
            }
        }
        break;

        case FdoDataType_Single:
        {
            if (bIsNull)
            {
                FdoSingleValue *val = FdoSingleValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoDoubleValue* value = dynamic_cast<FdoDoubleValue*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
                expr = FdoSingleValue::Create((float)value->GetDouble());
            }
        }
        break;

        case FdoDataType_Double:
        {
            if (bIsNull)
            {
                FdoDoubleValue *val = FdoDoubleValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoDoubleValue*value = dynamic_cast<FdoDoubleValue*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
            }
        }
        break;

        case FdoDataType_DateTime:
        {
            if (bIsNull)
            {
                FdoDateTimeValue *val = FdoDateTimeValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoDateTimeValue* value = dynamic_cast<FdoDateTimeValue*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
            }
        }
        break;

        case FdoDataType_Decimal:
        {
            if (bIsNull)
            {
                FdoDecimalValue *val = FdoDecimalValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoDoubleValue* value = dynamic_cast<FdoDoubleValue*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
                expr = FdoDecimalValue::Create(value->GetDouble());
            }
        }
        break;

        case FdoDataType_String:
        {
            if (bIsNull)
            {
                FdoStringValue *val = FdoStringValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                FdoStringValue* value = dynamic_cast<FdoStringValue*>(expr.p);
                if (value==NULL)  throw FdoException::Create(L"Wrong data type!");
            }
        }
        break;

        case FdoDataType_BLOB:
        {
            // NOTE: blob parsing doesn't work yet (ever?) in FDO, so we do our own custom parsing,
            // as comma-separated byte values:

            if (0==FdoCommonOSUtil::wcsicmp(data, L"NULL"))
            {
                FdoBLOBValue *val = FdoBLOBValue::Create();
                val->SetNull();
                expr = val;
            }
            else
            {
                // NOTE: Since FdoByteArray::Append() must be reassigned to the original variable,
                //       using FdoPtr on blobArray would cause its refcount to go to 0, so we can't use FdoPtr.
                FdoByteArray* blobArray = FdoByteArray::Create();
#ifdef _WIN32
                wchar_t *dataClone = _wcsdup(data);
#else
                wchar_t *dataClone = wcsdup(data);
#endif

                wchar_t *token = NULL;
                wchar_t *separators = L",";
                wchar_t *state = NULL;
                token = FdoCommonOSUtil::wcstok(dataClone, separators, &state);
                while (NULL != token)
                {
                    blobArray = FdoByteArray::Append(blobArray, (FdoByte) FdoCommonOSUtil::wtoi(token));
                    token = FdoCommonOSUtil::wcstok(NULL, separators, &state);
                }
                free(dataClone);

                expr = FdoBLOBValue::Create(blobArray);
                blobArray->Release();
            }
        }
        break;

        case FdoDataType_CLOB:
        default:
            throw FdoException::Create(L"Unhandled data type!");
        break;
    }

    return FDO_SAFE_ADDREF(expr.p);
}


void ArcSDETests::checkEqual(FdoPtr<FdoIFeatureReader> reader, wchar_t* propertyName, FdoDataType propertyType, const wchar_t* propertyData)
{
    // Get the literal expression we're comparing against:
    FdoPtr<FdoExpression> expression = ArcSDETests::ParseByDataType(propertyData,propertyType);
    
    bool bReaderIsNull = reader->IsNull(propertyName);

    switch (propertyType)
    {
        case FdoDataType_String:
            {
                FdoPtr<FdoStringValue> value = dynamic_cast<FdoStringValue*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                    CPPUNIT_ASSERT_MESSAGE ("incorrect string value", 0 == wcscmp (value->GetString (), reader->GetString (propertyName)));
            }
            break;
        case FdoDataType_Single:
            {
                FdoPtr<FdoSingleValue> value = dynamic_cast<FdoSingleValue*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                {
                    double p = value->GetSingle ();
                    double q = reader->GetSingle (propertyName);
                    double x = fabs (1.0 - (q / p));
                    CPPUNIT_ASSERT_MESSAGE ("incorrect single value", 1e-3 > x);
                }
            }
            break;
        case FdoDataType_Double:
            {
                FdoPtr<FdoDoubleValue> value = dynamic_cast<FdoDoubleValue*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                {
                    double p = value->GetDouble ();
                    double q = reader->GetDouble (propertyName);
                    double x = fabs (1.0 - (q / p));
                    CPPUNIT_ASSERT_MESSAGE ("incorrect double value", 1e-5 > x);
                }
            }
            break;
        case FdoDataType_Decimal:
            {
                FdoPtr<FdoDecimalValue> value = dynamic_cast<FdoDecimalValue*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                {
                    double p = value->GetDecimal ();
                    double q = reader->GetDouble (propertyName);
                    double x = fabs (1.0 - (q / p));
                    CPPUNIT_ASSERT_MESSAGE ("incorrect decimal value", 1e-5 > x);
                }
            }
            break;
        case FdoDataType_Int16:
            {
                FdoPtr<FdoInt16Value> value = dynamic_cast<FdoInt16Value*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                {
                    int p = value->GetInt16 ();
                    int q = reader->GetInt16 (propertyName);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect short value", p == q);
                }
            }
            break;
        case FdoDataType_Int32:
            {
                FdoPtr<FdoInt32Value> value = dynamic_cast<FdoInt32Value*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                {
                    int p = value->GetInt32 ();
                    int q = reader->GetInt32 (propertyName);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect integer value", p == q);
                }
            }
            break;
        case FdoDataType_DateTime:
            {
                FdoPtr<FdoDateTimeValue> value = dynamic_cast<FdoDateTimeValue*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                {
                    FdoDateTime t1 = value->GetDateTime ();
                    FdoDateTime t2 = reader->GetDateTime (propertyName);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect date year value", t1.year == t2.year);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect date month value", t1.month == t2.month);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect date day value", t1.day == t2.day);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect date hour value", t1.hour == t2.hour);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect date minute value", t1.minute == t2.minute);
                    /*ToDo CPPUNIT_ASSERT_MESSAGE ("incorrect date seconds value", t1.seconds == t2.seconds);*/
                }
            }
            break;
        case FdoDataType_Boolean:
            {
                FdoPtr<FdoBooleanValue> value = dynamic_cast<FdoBooleanValue*>(FDO_SAFE_ADDREF(expression.p));
                if ((value->IsNull() && !bReaderIsNull) || (!value->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!value->IsNull())
                {
                    bool b1 = value->GetBoolean ();
                    bool b2 = reader->GetBoolean (propertyName);
                    CPPUNIT_ASSERT_MESSAGE ("incorrect boolean value", b1 == b2);
                }
            }
            break;
        case FdoDataType_BLOB:
            {
                FdoPtr<FdoBLOBValue> blob1 = dynamic_cast<FdoBLOBValue*>(FDO_SAFE_ADDREF(expression.p));
                if ((blob1->IsNull() && !bReaderIsNull) || (!blob1->IsNull() && bReaderIsNull))
                    CPPUNIT_ASSERT_MESSAGE("Null value mismatch", false);
                else if (!blob1->IsNull())
                {
                    FdoPtr<FdoBLOBValue> blob2 = dynamic_cast<FdoBLOBValue*>(reader->GetLOB(propertyName));
                    FdoPtr<FdoByteArray> blob1Array = blob1->GetData();
                    FdoPtr<FdoByteArray> blob2Array = blob2->GetData();

                    CPPUNIT_ASSERT_MESSAGE ("incorrect blob size", blob1Array->GetCount() == blob2Array->GetCount());
                    for (FdoInt32 i=0; i<blob2Array->GetCount(); i++)
                    {
                        FdoByte blob1Byte = (*blob1Array)[i];
                        FdoByte blob2Byte = (*blob2Array)[i];
                        CPPUNIT_ASSERT_MESSAGE ("incorrect blob value", blob1Byte == blob2Byte);
                    }
                }
            }
            break;
        default:
            CPPUNIT_FAIL ("unknown data type");
    }
}

bool ArcSDETests::fuzzyEqual (const double d1, const double d2)
{
    if ((d1==0.0) || (d2==0.0))
        return 1e-5 > fabs(d1 - d2);
    else
        return 1e-5 > fabs(1.0 - (d2 / d1));
}


// Accesses all the data on the current feature.
void ArcSDETests::ProcessFeature (FdoIFeatureReader* featureReader, FdoIdentifierCollection* IDs)
{
    FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)featureReader->GetClassDefinition();
    FdoPtr<FdoPropertyDefinitionCollection> propColl = classDef->GetProperties();
	bool bShouldBeIn = false;
	bool bFound = false;
    bool bIsNull = false;


    // access all the data properties
    for (int i=0; i<propColl->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> prop = propColl->GetItem(i);
        FdoString* propName = prop->GetName();
		bFound = false;

		// Determine if this property should be in the reader or not:
		bShouldBeIn = false;
		if (NULL==IDs)
			bShouldBeIn = true;
		else
		{
			for (FdoInt32 i=0; i<IDs->GetCount(); i++)
			{
				FdoPtr<FdoIdentifier> ID = IDs->GetItem(i);
				if (0==wcscmp(propName, ID->GetName()))
				{
					bShouldBeIn = true;
					break;
				}
			}
		}

        try
        {
            bIsNull = featureReader->IsNull(propName);
        }
        catch (FdoException *e)
        {
            bFound = false;
            e->Release();
        }

        if (bIsNull)
        {
            bFound = true;
        }
        else if (prop->GetPropertyType() == FdoPropertyType_DataProperty)
		{
			FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)prop.p;
			FdoDataType propDataType = dataProp->GetDataType();

			try
			{
				switch (propDataType)
				{
					case FdoDataType_Boolean:
						featureReader->GetBoolean(propName);
						break;
					case FdoDataType_Byte:
						featureReader->GetByte(propName);
						break;
					case FdoDataType_DateTime:
						featureReader->GetDateTime(propName);
						break;
					case FdoDataType_Decimal:
						featureReader->GetDouble(propName);
						break;
					case FdoDataType_Double:
						featureReader->GetDouble(propName);
						break;
					case FdoDataType_Int16:
						featureReader->GetInt16(propName);
						break;
					case FdoDataType_Int32:
						featureReader->GetInt32(propName);
						break;
					case FdoDataType_Int64:
						featureReader->GetInt64(propName);
						break;
					case FdoDataType_Single:
						featureReader->GetSingle(propName);
						break;
					case FdoDataType_String:
						featureReader->GetString(propName);
						break;
					case FdoDataType_BLOB:
						CPPUNIT_FAIL("Need to test BLOB properly!");
						//TODO:featureReader->GetBLOB(propName);
						break;
					/* NOT SUPPORTD IN R1:
					case FdoDataType_CLOB:
						featureReader->GetCLOB(propName);
						break;
					*/
				}

				bFound = true;
			}
			catch (FdoException *e)
			{
				bFound = false;
				e->Release();
			}
		}
		else if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
		{
			// access the geometric property
			FdoPtr<FdoGeometricPropertyDefinition> geomProp = classDef->GetGeometryProperty();
			if (0!=wcscmp(propName, geomProp->GetName()))
				CPPUNIT_FAIL("Expecting at most one geometric property, found at least two");

			// access the FGF geometric data
			FdoPtr<FdoByteArray> fgf;
			try
			{
				fgf = featureReader->GetGeometry(propName);
				bFound = true;
			}
			catch (FdoException *e)
			{
				bFound = false;
				e->Release();
			}

			if (bFound)
			{
				FdoInt32 numBytes = fgf->GetCount();

				// make a geometry
				FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
				FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf(fgf);
				FdoPtr<FdoIEnvelope> bbox = geom->GetEnvelope();
			}
		}

		// fail if this property SHOULD NOT have been in the reader:
		if (bFound && !bShouldBeIn)
			CPPUNIT_FAIL("Property should NOT have been in the reader, but was.");

		// fail if this property SHOULD have been in the reader:
		if (!bFound && bShouldBeIn)
			CPPUNIT_FAIL("Property SHOULD have been in the reader, but was not.");
    }
}




void ArcSDETests::ActivateSpatialContext(FdoIConnection* connection, FdoString* scName)
{
    try
    {
        FdoPtr<FdoIActivateSpatialContext> activateSC = (FdoIActivateSpatialContext*)connection->CreateCommand (FdoCommandType_ActivateSpatialContext);
        activateSC->SetName(scName);
        activateSC->Execute();
    }
    catch (FdoException *e)
    {
        // We ignore errors here, because the given spatial context may no longer exist
        e->Release();
    }
}



bool ArcSDETests::GeometriesEquivalent(FdoIGeometry *geom1, FdoIGeometry *geom2)
{
    FdoInt32 dummyInt32 = 0;
    bool bRet = true;
    FdoGeometryType type1 = geom1->GetDerivedType();
    FdoGeometryType type2 = geom2->GetDerivedType();
    FdoDimensionality dim1 = (FdoDimensionality)geom1->GetDimensionality();
    FdoDimensionality dim2 = (FdoDimensionality)geom1->GetDimensionality();

    // Make sure types are the same:
    if (type1 != type2)
        return false;

    // Make sure dimensionalities are the same:
    if (dim1 != dim2)
        return false;

    // Make sure contents are the same:
    FdoPtr<FdoIDirectPosition> pos1;
    FdoPtr<FdoIDirectPosition> pos2;
    bool bHasZ = (dim1 & FdoDimensionality_Z) > 0;
    bool bHasM = (dim1 & FdoDimensionality_M) > 0;
    switch (type1)
    {
        case FdoGeometryType_Point:
        {
            FdoIPoint* p1 = (FdoIPoint*)geom1;
            FdoIPoint* p2 = (FdoIPoint*)geom2;
            pos1 = p1->GetPosition();
            pos2 = p2->GetPosition();
            if (!PointsEquivalent(pos1, pos2))
                return false;
        }
        break;

        case FdoGeometryType_LineString:
        {
            FdoILineString* l1 = (FdoILineString*)geom1;
            FdoILineString* l2 = (FdoILineString*)geom2;
            FdoInt32 iCount1 = l1->GetCount();
            FdoInt32 iCount2 = l2->GetCount();
            if (iCount1 != iCount2)
                return false;
            for (FdoInt32 i=0; i<iCount1; i++)
            {
                pos1 = l1->GetItem(i);
                pos2 = l2->GetItem(i);
                if (!PointsEquivalent(pos1, pos2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_Polygon:
        {
            FdoIPolygon* p1 = (FdoIPolygon*)geom1;
            FdoIPolygon* p2 = (FdoIPolygon*)geom2;
            FdoPtr<FdoILinearRing> ring1 = p1->GetExteriorRing();
            FdoPtr<FdoILinearRing> ring2 = p2->GetExteriorRing();
            if (!LinearRingsEquivalent(ring1, ring2))
                return false;
            FdoInt32 iIntRingCount1 = p1->GetInteriorRingCount();
            FdoInt32 iIntRingCount2 = p2->GetInteriorRingCount();
            if (iIntRingCount1 != iIntRingCount2)
                return false;
            for (FdoInt32 i=0; i<iIntRingCount1; i++)
            {
                ring1 = p1->GetInteriorRing(i);
                ring2 = p2->GetInteriorRing(i);
                if (!LinearRingsEquivalent(ring1, ring2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiPoint:
        {
            FdoIMultiPoint* multi1 = (FdoIMultiPoint*)geom1;
            FdoIMultiPoint* multi2 = (FdoIMultiPoint*)geom2;
            FdoInt32 count1 = multi1->GetCount();
            FdoInt32 count2 = multi2->GetCount();
            if (count1 != count2)
                return false;
            for (FdoInt32 i=0; i<count1; i++)
            {
                FdoPtr<FdoIGeometry> item1 = multi1->GetItem(i);
                FdoPtr<FdoIGeometry> item2 = multi2->GetItem(i);
                if (!GeometriesEquivalent(item1,item2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiLineString:
        {
            FdoIMultiLineString* multi1 = (FdoIMultiLineString*)geom1;
            FdoIMultiLineString* multi2 = (FdoIMultiLineString*)geom2;
            FdoInt32 count1 = multi1->GetCount();
            FdoInt32 count2 = multi2->GetCount();
            if (count1 != count2)
                return false;
            for (FdoInt32 i=0; i<count1; i++)
            {
                FdoPtr<FdoIGeometry> item1 = multi1->GetItem(i);
                FdoPtr<FdoIGeometry> item2 = multi2->GetItem(i);
                if (!GeometriesEquivalent(item1,item2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiPolygon:
        {
            FdoIMultiPolygon* multi1 = (FdoIMultiPolygon*)geom1;
            FdoIMultiPolygon* multi2 = (FdoIMultiPolygon*)geom2;
            FdoInt32 count1 = multi1->GetCount();
            FdoInt32 count2 = multi2->GetCount();
            if (count1 != count2)
                return false;
            for (FdoInt32 i=0; i<count1; i++)
            {
                FdoPtr<FdoIGeometry> item1 = multi1->GetItem(i);
                FdoPtr<FdoIGeometry> item2 = multi2->GetItem(i);
                if (!GeometriesEquivalent(item1,item2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiGeometry:
        case FdoGeometryType_CurveString:
        case FdoGeometryType_CurvePolygon:
        case FdoGeometryType_MultiCurveString:
        case FdoGeometryType_MultiCurvePolygon:
        default:
            throw FdoException::Create(L"Don't know how to compare these 2 geometries");
        break;
    }

    return bRet;
}


bool ArcSDETests::PointsEquivalent(FdoIDirectPosition* pos1, FdoIDirectPosition* pos2)
{
    if (pos1->GetDimensionality() != pos2->GetDimensionality())
        return false;

    bool bHasZ = (pos1->GetDimensionality() & FdoDimensionality_Z) > 0;
    bool bHasM = (pos1->GetDimensionality() & FdoDimensionality_M) > 0;

    if (!fuzzyEqual(pos1->GetX(), pos2->GetX()))
        return false;
    if (!fuzzyEqual(pos1->GetY(), pos2->GetY()))
        return false;
    if (bHasZ && !fuzzyEqual(pos1->GetZ(),pos2->GetZ()))
        return false;
    if (bHasM && !fuzzyEqual(pos1->GetM(),pos2->GetM()))
        return false;

    return true;
}


bool ArcSDETests::LinearRingsEquivalent(FdoILinearRing* lr1, FdoILinearRing* lr2)
{
    FdoInt32 count1 = lr1->GetCount();
    FdoInt32 count2 = lr2->GetCount();
    if (count1 != count2)
        return false;

    // Compare all the points in the 2 linear rings:
    for (FdoInt32 i=0; i<count1; i++)
    {
        FdoPtr<FdoIDirectPosition> pos1 = lr1->GetItem(i);
        FdoPtr<FdoIDirectPosition> pos2 = lr2->GetItem(i);
        if (!PointsEquivalent(pos1,pos2))
            return false;
    }

    return true;
}


bool ArcSDETests::CreateSchemaOnly(void)
{
   bool bCreateSchema = false;
   char *initSchema = getenv("initschema");
   if (initSchema)
   {
      wchar_t *initSchemaW;
      multibyte_to_wide(initSchemaW, initSchema);
      bCreateSchema = FdoCommonOSUtil::wcsicmp(initSchemaW, L"true") == 0;
   }
   return bCreateSchema;
}


void ArcSDETests::set_extent(FdoICreateSpatialContext *cmdCreateSpatialContext, double minx, double miny, double maxx, double maxy)
{
    // Convert minx/miny/maxx/maxy to ordinate array:
    const long ordinateCount = 5*2;  // 5 points * 2 ordinates per point
    double ordinates[ordinateCount];  
    ordinates[0] = minx;    ordinates[1] = miny;
    ordinates[2] = minx;    ordinates[3] = maxy;
    ordinates[4] = maxx;    ordinates[5] = maxy;
    ordinates[6] = maxx;    ordinates[7] = miny;
    ordinates[8] = minx;    ordinates[9] = miny;

    // Convert ordinate array to FGF polygon:
    FdoPtr<FdoFgfGeometryFactory> fgfFactory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> exteriorRing = fgfFactory->CreateLinearRing(FdoDimensionality_XY, ordinateCount, ordinates);
    FdoPtr<FdoIPolygon> polygon = fgfFactory->CreatePolygon(exteriorRing, NULL);
    FdoPtr<FdoByteArray> byteArray = fgfFactory->GetFgf(polygon);

    // Set extent to constructed polygon:
    cmdCreateSpatialContext->SetExtent(byteArray);
}

void ArcSDETests::get_extent(FdoISpatialContextReader *spatialContextReader, double &minx, double &miny, double &maxx, double &maxy)
{
    // Get extent:
    FdoPtr<FdoByteArray> fgfArray = spatialContextReader->GetExtent();

    // Convert spatial context's extent to a FdoIEnvelope:
    FdoPtr<FdoFgfGeometryFactory> fgfFactory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geom = fgfFactory->CreateGeometryFromFgf(fgfArray);
    FdoPtr<FdoIEnvelope>  geomEnvelope = geom->GetEnvelope();

    // Convert envelope to minx/miny/maxx/maxy:
    minx = geomEnvelope->GetMinX();
    miny = geomEnvelope->GetMinY();
    maxx = geomEnvelope->GetMaxX();
    maxy = geomEnvelope->GetMaxY();
}



void ArcSDETests::CreateOrUpdateSpatialContext(FdoIConnection *conn, FdoString* scName, double dXMin, double dYMin, double dXMax, double dYMax, FdoString* scCoordSys)
{
    // Determine if the spatial context already exists:
    FdoPtr<FdoIGetSpatialContexts> getSCs = (FdoIGetSpatialContexts*)conn->CreateCommand(FdoCommandType_GetSpatialContexts);
    FdoPtr<FdoISpatialContextReader> scReader = getSCs->Execute();
    bool bExists = false;
    while (scReader->ReadNext())
    {
        if (0==wcscmp(scReader->GetName(), scName))
        {
            bExists = true;
            break;
        }
    }
    scReader = NULL;

    // Create the spatial context (or update it if it already exists):
    FdoPtr<FdoICreateSpatialContext> createSC = (FdoICreateSpatialContext*)conn->CreateCommand(FdoCommandType_CreateSpatialContext);
    createSC->SetName(scName);
    set_extent(createSC, dXMin, dYMin, dXMax, dYMax);
    createSC->SetCoordinateSystem(scCoordSys);
    createSC->SetUpdateExisting(bExists);
    createSC->Execute();
}


int ArcSDETests::GetLockCount(FdoIConnection *conn, FdoString* userName, FdoString* qualified_class_name)
{
    FdoInt32 lockCount = 0;

    FdoPtr <FdoIGetLockedObjects> get = (FdoIGetLockedObjects*)conn->CreateCommand (FdoCommandType_GetLockedObjects);
    get->SetLockOwner(userName);
    FdoPtr<FdoILockedObjectReader> locked = get->Execute ();
    while (locked->ReadNext ())
    {
        if (0==wcscmp(locked->GetFeatureClassName(), qualified_class_name))
            lockCount++;
    }

    return lockCount;
}


void ArcSDETests::PrintAndThrow(FdoString* message)
{
    printf("\n%ls\n", message);
    throw FdoException::Create(message);
}

