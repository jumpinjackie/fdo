/*
 * 
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
#include "ShpTests.h"
#include <FdoSpatial.h>

#include <FdoCommonFile.h>

#define SHP_MESSAGE_DEFINE

#include <../Message/ShpMessageStatic.h>
#include <../Message/Inc/ShpMessage.h>

bool ShpTests::VERBOSE = false;
wchar_t* ShpTests::sLocation = L"";

ShpTests::ShpTests(void)
{
}

ShpTests::~ShpTests(void)
{
}

FdoIConnection* ShpTests::GetConnection ()
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    return (manager->CreateConnection (L"OSGeo.SHP"));
}



void ShpTests::Expect (FdoIConnection* connection, UnitTestClass* definition, std::vector<int*> *map)
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
    select->SetFeatureClassName (identifier->GetName ());
    reader = select->Execute ();
    got = 0;
    while (reader->ReadNext ())
    {
        identity = (int)reader->GetDouble (definition->mProperties[definition->mIdIndex]->mPropertyName);
        found = false;
        for (size_t i = 0; i < map->size (); i++)
            if ((*map)[i][1] == identity)
            {
                for (size_t j = 0; j < definition->mProperties.size (); j++)
                    if (definition->mIdIndex != j)
                    {
                        size_t index = (*map)[i][0];
                        if (definition->mProperties[j]->mPropertyData.size () > index)
                            value = ShpTests::ParseByDataType (definition->mProperties[j]->mPropertyData[index], definition->mProperties[j]->mPropertyType);
                        else
                            value = ShpTests::ParseByDataType (L"null", definition->mProperties[j]->mPropertyType);
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


FdoExpression* ShpTests::ParseByDataType(const wchar_t* data, FdoDataType dataType)
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
                FdoDoubleValue* valueDouble = dynamic_cast<FdoDoubleValue*>(expr.p);
                if (NULL != valueDouble)
                    expr = FdoDecimalValue::Create(valueDouble->GetDouble());
                else
                {
                    FdoInt32Value* valueInt32 = dynamic_cast<FdoInt32Value*>(expr.p);
                    if (NULL != valueInt32)
                        expr = FdoDecimalValue::Create((double)valueInt32->GetInt32());
                    else
                        throw FdoException::Create(L"Wrong data type!");
                }
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


void ShpTests::checkEqual(FdoPtr<FdoIFeatureReader> reader, wchar_t* propertyName, FdoDataType propertyType, const wchar_t* propertyData)
{
    // Get the literal expression we're comparing against:
    FdoPtr<FdoExpression> expression = ShpTests::ParseByDataType(propertyData,propertyType);
    
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

// Accesses all the data on the current feature.
void ShpTests::ProcessFeature (FdoIFeatureReader* featureReader, FdoIdentifierCollection* IDs)
{
    FdoPtr<FdoClassDefinition> classDef = featureReader->GetClassDefinition();
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
            FdoFeatureClass* featureClassDef = dynamic_cast<FdoFeatureClass*>(classDef.p);
            if (featureClassDef)
            {
    			FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClassDef->GetGeometryProperty();
    			if (0!=wcscmp(propName, geomProp->GetName()))
	    			CPPUNIT_FAIL("Expecting at most one geometric property, found at least two");
            }

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




void ShpTests::ActivateSpatialContext(FdoIConnection* connection, FdoString* scName)
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



wchar_t* ShpTests::ColumnTypeToString (eDBFColumnType type)
{
    wchar_t* ret;

    switch (type)
    {
        case kColumnUnsupportedType:
            ret = L"Unsupported";
            break;
        case kColumnCharType:
            ret = L"CHAR";
            break;
        case kColumnDecimalType:
            ret = L"DECIMAL";
            break;
        case kColumnDateType:
            ret = L"DATE";
            break;
        case kColumnLogicalType:
            ret = L"LOGICAL";
            break;
        default:
            CPPUNIT_FAIL ("bad type");
    }

    return (ret);
}



/// <summary>Copy a class.</summary>
/// <param name="target_connection">A connection for the target class.</param> 
/// <param name="target_class">The class name for the duplicate class.</param> 
/// <param name="source_class">The class to duplicate.</param> 
/// <param name="new_schema">If true, a new schema ("TheSchema") is created for the new class.</param> 
void ShpTests::copy_schema (FdoIConnection* target_connection, FdoString* target_class, FdoFeatureClass* source_class, bool new_schema)
{
    // delete the class, if its there:
    TestCommonSchemaUtil::CleanUpClass(target_connection, NULL, target_class);

    // create the class
    FdoPtr<FdoFeatureSchema> schema;
    
    if (new_schema)
        schema = FdoFeatureSchema::Create (L"TheSchema", L" test schema");
    else
    {
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)target_connection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        schema = schemas->GetItem (0);
    }
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();


    // add every property from the source to the destination
    FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (target_class, L"test class created with apply schema");
    FdoPtr<FdoPropertyDefinitionCollection> dest_properties = feature->GetProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> dest_identities = feature->GetIdentityProperties ();
    FdoPtr<FdoPropertyDefinitionCollection> source_properties = source_class->GetProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> source_identities = source_class->GetIdentityProperties ();
    for (FdoInt32 i = 0; i < source_properties->GetCount (); i++)
    {
        FdoPtr<FdoPropertyDefinition> source_definition = source_properties->GetItem (i);
        if (source_definition->GetPropertyType () == FdoPropertyType_DataProperty)
        {
            FdoDataPropertyDefinition* source_data_definition = (FdoDataPropertyDefinition*)(source_definition.p);
            FdoPtr<FdoDataPropertyDefinition> dest_data_definition = FdoDataPropertyDefinition::Create (source_data_definition->GetName (), source_data_definition->GetDescription ());
            dest_data_definition->SetDataType (source_data_definition->GetDataType ());
            dest_data_definition->SetIsAutoGenerated (source_data_definition->GetIsAutoGenerated ());
            dest_data_definition->SetNullable (source_data_definition->GetNullable ());
            dest_data_definition->SetLength (source_data_definition->GetLength ());
            dest_data_definition->SetPrecision (source_data_definition->GetPrecision ());
            dest_data_definition->SetScale (source_data_definition->GetScale ());
            dest_properties->Add (dest_data_definition);
            if (FdoPtr<FdoDataPropertyDefinition> (source_identities->FindItem (source_data_definition->GetName ())) != NULL)
                dest_identities->Add (dest_data_definition);
        }
        else if (source_definition->GetPropertyType () == FdoPropertyType_ObjectProperty)
        {
            throw FdoException::Create (L"object properties?");
        }
        else if (source_definition->GetPropertyType () == FdoPropertyType_GeometricProperty)
        {
            FdoGeometricPropertyDefinition* source_geometry_definition = (FdoGeometricPropertyDefinition*)(source_definition.p);
            FdoPtr<FdoGeometricPropertyDefinition> dest_geometry_definition = FdoGeometricPropertyDefinition::Create (source_geometry_definition->GetName (), source_geometry_definition->GetDescription ());
            dest_geometry_definition->SetGeometryTypes (source_geometry_definition->GetGeometryTypes ());
            dest_geometry_definition->SetHasElevation (source_geometry_definition->GetHasElevation ());
            dest_geometry_definition->SetHasMeasure (source_geometry_definition->GetHasMeasure ());
            dest_properties->Add (dest_geometry_definition);
            feature->SetGeometryProperty (dest_geometry_definition);
        }
    }

    // submit the new schema
    classes->Add (feature);
    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)target_connection->CreateCommand (FdoCommandType_ApplySchema);
    apply->SetFeatureSchema (schema);
    apply->Execute ();
	SaveSchema(target_connection);
}

/// <summary>Duplicate a class.</summary>
/// <param name="target_connection">A connection for the target class.</param> 
/// <param name="target_class">The class name for the duplicate class.</param> 
/// <param name="file_name">Pass the file set name (full path but no extension).</param> 
/// <param name="ignore_states">If true use SetIgnoreStates on the source schema and apply it
/// to the new schema, else manually copy all the class definitions to the target schema.</param> 
void ShpTests::copy (FdoIConnection* target_connection, FdoString* target_class, const wchar_t* file_name, bool ignore_states)
{
    clock_t start;
    clock_t finish;
    int count;
    wchar_t* location;
    wchar_t* class_name;
    const wchar_t* format = L"DefaultFileLocation=%ls";
    wchar_t* connect_string;
    bool dup;
    FdoPtr<FdoClassDefinition> cls;
    FdoPtr<FdoPropertyDefinitionCollection> properties;
    FdoString* name;
    FdoPtr<FdoDataValue> value;
    size_t length;

    start = clock ();

    // split into location and class name
    location = (wchar_t*)alloca (sizeof(wchar_t) * (wcslen (file_name) + 1));
    wcscpy (location, file_name);
    class_name = wcsrchr (location, L'\\');
    if (NULL == class_name)
        class_name = wcsrchr (location, L'/');
    *class_name = L'\0';
    class_name++;

    length = sizeof(wchar_t) * (wcslen (location) + wcslen (format));
    connect_string = (wchar_t*)alloca (length);
    FdoCommonOSUtil::swprintf (connect_string, length, format, location);
    FdoPtr<FdoIConnection> connection = ShpTests::GetConnection ();
    connection->SetConnectionString (connect_string);
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());

	// Create a new spatial context
    FdoPtr<FdoICreateSpatialContext> cscCmd = (FdoICreateSpatialContext *)target_connection->CreateCommand( FdoCommandType_CreateSpatialContext );

    cscCmd->SetName(L""); // Leave it empty to avoid confict with WKT
    cscCmd->SetDescription(L"test");
    cscCmd->SetCoordinateSystem(L""); // Leave it empty to avoid confict with WKT
    cscCmd->SetUpdateExisting(false);
    cscCmd->SetXYTolerance(.02);
    cscCmd->SetZTolerance(.001);
    
	// Copy the spatial contexts from source to target
	FdoPtr<FdoIGetSpatialContexts> spatialContexts = (FdoIGetSpatialContexts*)connection->CreateCommand (FdoCommandType_GetSpatialContexts);
    FdoPtr<FdoISpatialContextReader> pScReader = spatialContexts->Execute();
    int cnt = 0;
	FdoStringP	scname;

    while (pScReader->ReadNext())
    {
		FdoStringP	scName = pScReader->GetName();
		FdoStringP	wkt = pScReader->GetCoordinateSystemWkt();

		if ( wkt != L"" )
		{
			cscCmd->SetCoordinateSystemWkt(wkt);
			cscCmd->Execute();
		}
    }

	// Now copy the classes
    FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)connection->CreateCommand (FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
    dup = false;
    for (int i = 0; !dup && i < schemas->GetCount (); i++)
    {
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (i);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        for (int j = 0; !dup && j < classes->GetCount (); j++)
        {
            cls = classes->GetItem (j);
            if (0 == wcscmp (cls->GetName (), class_name))
            {
                if (ignore_states)
                {
                    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)target_connection->CreateCommand (FdoCommandType_ApplySchema);
                    apply->SetFeatureSchema (schema);
                    apply->SetIgnoreStates (true);
                    apply->Execute ();
					SaveSchema(target_connection);
                    dup = true;
                }
                else
                {
                    FdoFeatureClass* feature = dynamic_cast<FdoFeatureClass*>(cls.p);
                    if (NULL != feature)
                    {
                        copy_schema (target_connection, target_class, feature, false);
                        dup = true;
                    }
                }
            }
        }
    }

    CPPUNIT_ASSERT_MESSAGE ("class duplication failed", dup);

    FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (class_name);
    FdoPtr<FdoIFeatureReader> reader = select->Execute ();
    FdoPtr<FdoIInsert> insert = (FdoIInsert*)target_connection->CreateCommand (FdoCommandType_Insert);
    insert->SetFeatureClassName (target_class);
    properties = cls->GetProperties ();
    count = 0;
    while (reader->ReadNext ())
    {
        count++;
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        for (int i = 0; i < properties->GetCount (); i++)
        {
            FdoPtr<FdoPropertyDefinition> definition = properties->GetItem (i);
            name = definition->GetName ();
            if (definition->GetIsSystem ())
                continue;
            if (definition->GetPropertyType () == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* data_definition = (FdoDataPropertyDefinition*)(definition.p);
                if (data_definition->GetIsAutoGenerated ())
                    continue;
                FdoPtr<FdoPropertyValue> data = FdoCommonMiscUtil::GetPropertyValue(name, FdoPropertyType_DataProperty, data_definition->GetDataType(), reader);
                values->Add (FdoPtr<FdoPropertyValue> (data));
            }
            else if (definition->GetPropertyType () == FdoPropertyType_ObjectProperty)
            {
                throw FdoException::Create (L"object properties?");
            }
            else if (definition->GetPropertyType () == FdoPropertyType_GeometricProperty)
            {
                // FdoPtr<FdoByteArray> (reader->GetGeometry (L"Geometry"));
                values->Add (FdoPtr<FdoPropertyValue> (FdoPtr<FdoPropertyValue> (FdoPropertyValue::Create (name, FdoPtr<FdoGeometryValue> (FdoGeometryValue::Create (FdoPtr<FdoByteArray>(reader->GetGeometry (name))))))));
            }
        }
        FdoPtr<FdoIFeatureReader> discard = insert->Execute ();
        values->Clear ();
    }
    connection->Close ();
    finish = clock ();
    double duration = (double)(finish - start) / CLOCKS_PER_SEC;
    if (VERBOSE)
        printf ("%d features in %2.3f seconds\n", count, duration);
}

void ShpTests::SaveSchema(FdoIConnection* connection)
{
	FdoStringP pLocation = ShpTests::sLocation;
	if (ShpTests::sLocation[pLocation.GetLength()-1] != FILE_PATH_DELIMITER)
	{
		wchar_t Sep[2];
		Sep[0] = FILE_PATH_DELIMITER;
		Sep[1] = L'\0';
		pLocation += Sep;
	}
	pLocation += L"schema.xml";
	FdoPtr <FdoXmlWriter> writersv = FdoXmlWriter::Create (pLocation, true, FdoXmlWriter::LineFormat_Indent, 128);
	
	FdoPtr<FdoIDescribeSchema> describe_schemasv = (FdoIDescribeSchema*)connection->CreateCommand (FdoCommandType_DescribeSchema);
	FdoPtr<FdoFeatureSchemaCollection> schemassv = describe_schemasv->Execute ();
	schemassv->WriteXml (writersv, NULL);
	FdoPtr<FdoIDescribeSchemaMapping> describe_mappingsv = (FdoIDescribeSchemaMapping*)connection->CreateCommand (FdoCommandType_DescribeSchemaMapping);
	FdoPtr<FdoPhysicalSchemaMappingCollection> mappingssv = describe_mappingsv->Execute ();
	if (0 != mappingssv->GetCount ())
		mappingssv->WriteXml (writersv, NULL);
	writersv = NULL;
}
