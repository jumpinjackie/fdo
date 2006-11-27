// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


#include "Pch.h"
#include "GmlWriteTest.h"

#include <Fdo/Xml/FeaturePropertyWriter.h>
#include <Fdo/Xml/FeatureSerializer.h>
#include <Fdo/Xml/FeatureReader.h>
#include <Fdo/Xml/FeatureFlags.h>
#include <Fdo/Xml/FeatureWriter.h>
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION (GmlWriteTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (GmlWriteTest, "GmlWriteTest");

GmlWriteTest::GmlWriteTest(void)
{
}

GmlWriteTest::~GmlWriteTest(void)
{
}

void GmlWriteTest::setUp ()
{
}

class MyFeatureReader : public FdoIFeatureReader {
private:
    int m_depth;
    int m_current;
    int m_numFeatures;
protected:
    virtual void Dispose() {
        delete this;
    }
    virtual ~MyFeatureReader() {}
public:
    MyFeatureReader(int depth = 0, int numFeatures = 2) : m_current(-1), m_depth(depth), m_numFeatures(numFeatures) {}

public:
    virtual FdoClassDefinition* GetClassDefinition() {
        FdoPtr<FdoFeatureClass> classDef = FdoFeatureClass::Create(L"myFeature", L"myFeatureDesc");
        FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();

        FdoPtr<FdoDataPropertyDefinition> dataProp = FdoDataPropertyDefinition::Create(L"boolProp", L"bool property");
        dataProp->SetDataType(FdoDataType_Boolean);
        props->Add(dataProp);

        dataProp = FdoDataPropertyDefinition::Create(L"int32Prop", L"int32 property");
        dataProp->SetDataType(FdoDataType_Int32);
        props->Add(dataProp);

        dataProp = FdoDataPropertyDefinition::Create(L"int64Prop", L"int64 property");
        dataProp->SetDataType(FdoDataType_Int64);
        props->Add(dataProp);

        dataProp = FdoDataPropertyDefinition::Create(L"doubleProp", L"double property");
        dataProp->SetDataType(FdoDataType_Double);
        props->Add(dataProp);

        dataProp = FdoDataPropertyDefinition::Create(L"decimalProp", L"decimal property");
        dataProp->SetDataType(FdoDataType_Decimal);
        props->Add(dataProp);

        dataProp = FdoDataPropertyDefinition::Create(L"datetimeProp", L"datetime property");
        dataProp->SetDataType(FdoDataType_DateTime);
        props->Add(dataProp);

        dataProp = FdoDataPropertyDefinition::Create(L"stringProp", L"string property");
        dataProp->SetDataType(FdoDataType_String);
        props->Add(dataProp);

        dataProp = FdoDataPropertyDefinition::Create(L"blobProp", L"blob property");
        dataProp->SetDataType(FdoDataType_BLOB);
        props->Add(dataProp);

        // geometric property
        FdoPtr<FdoGeometricPropertyDefinition> geoProp = FdoGeometricPropertyDefinition::Create(L"geoProp", L"geometric property");
        props->Add(geoProp);

        if (m_depth < 1) {
            // object property
            FdoPtr<FdoObjectPropertyDefinition> objProp = FdoObjectPropertyDefinition::Create(L"objProp", L"object property");
            props->Add(objProp);

            // association property
            FdoPtr<FdoAssociationPropertyDefinition> associationProp = FdoAssociationPropertyDefinition::Create(L"associationProp", L"association property");
            props->Add(associationProp);

            // association1 property
            FdoPtr<FdoAssociationPropertyDefinition> associationProp1 = FdoAssociationPropertyDefinition::Create(L"associationProp1", L"association property1");
            props->Add(associationProp1);
        }
        
        return FDO_SAFE_ADDREF(classDef.p);

    }
    virtual FdoInt32 GetDepth() {
        return 0;
    }
    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count) {
        FdoPtr<FdoByteArray> geo = GetGeometry(propertyName);
        FDO_SAFE_ADDREF(geo.p);
        *count = geo->GetCount();
        return geo->GetData();
    }
    virtual FdoByteArray* GetGeometry(FdoString* propertyName) {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIEnvelope> pEnvelope = gf->CreateEnvelopeXY(123.33, 234.44, 456.66, 678.99);
        FdoPtr<FdoIGeometry> pGeometry = gf->CreateGeometry(pEnvelope);
	    FdoGeometryType type = pGeometry->GetDerivedType();
        FdoPtr<FdoByteArray> pByteArray = gf->GetFgf(pGeometry);
        return FDO_SAFE_ADDREF(pByteArray.p);
    }

    virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName) {
        if (wcscmp(L"associationProp1", propertyName) == 0)
            return new MyFeatureReader(m_depth + 1, 1);
        else
            return new MyFeatureReader(m_depth + 1);
    }

    virtual bool GetBoolean(FdoString* propertyName) {
        switch (m_current) {
            case 0:
                return false;
            default:
                return true;
        }
    }

    virtual FdoByte GetByte(FdoString* propertyName) {
        return 0xe3;
    }
    virtual FdoDateTime GetDateTime(FdoString* propertyName) {
        switch (m_current) {
            case 0:
                return FdoDateTime(2005, 6, (FdoInt8)18);
            case 1:
                return FdoDateTime(18, 29, 33.4567f);
            default:
                return FdoDateTime(2005, 6, 18, 18, 29, 33.0f);
        }
        
    }

    virtual double GetDouble(FdoString* propertyName) {
        return 4589.6766346456409;
    }

    virtual FdoInt16 GetInt16(FdoString* propertyName) {
        return 3245;
    }

    virtual FdoInt32 GetInt32(FdoString* propertyName) {
        return 453456;
    }

    virtual FdoInt64 GetInt64(FdoString* propertyName) {
        return (FdoInt64)0xffffffff * 10;
    }

    virtual float GetSingle(FdoString* propertyName) {
        return 342354.323f;
    }

    virtual FdoString* GetString(FdoString* propertyName) {
        return L"Test String";
    }

    virtual FdoLOBValue* GetLOB(FdoString* propertyName) {
        const char* bin = "An infestation of locusts that decimated crops, the worst drought in 15 years, and Niger's government keeping grain prices too high for too long have now caused a region in the brink of a crisis.";
        FdoPtr<FdoByteArray> ba = FdoByteArray::Create((const unsigned char*)bin, (FdoInt32)strlen(bin));
        return FdoBLOBValue::Create(ba);
    }

    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName ) {
        return NULL;
    }

    virtual bool IsNull(FdoString* propertyName) {
        return false;
    }

    virtual FdoIRaster* GetRaster(FdoString* propertyName) {
        return NULL;
    }
    virtual bool ReadNext() {
        if (++m_current > m_numFeatures - 1)
            return false;
        return true;
    }

    virtual void Close() {
    }
};

void GmlWriteTest::testSimpleFeature() {
		
	FdoPtr<FdoIFeatureReader> featureReader = new MyFeatureReader();

    FdoPtr<FdoXmlWriter> xmlWriter = FdoXmlWriter::Create(L"gml_write.xml", false);
    FdoPtr<FdoXmlFeaturePropertyWriter> propWriter = FdoXmlFeaturePropertyWriter::Create(xmlWriter);

    FdoPtr<FdoXmlFeatureFlags> flags = FdoXmlFeatureFlags::Create();
    flags->SetWriteCollection(true);
    flags->SetWriteMember(true);
    flags->SetCollectionUri(L"http://www.opengis.net/wfs");
    flags->SetCollectionName(L"FeatureCollection");
    flags->SetMemberName(L"featureMember");
    flags->SetMemberUri(L"http://www.opengis.net/gml");
    // set schemaLocation
    // gml schema location
    flags->SetSchemaLocation(L"http://www.opengis.net/gml", L"http://schemas.opengis.net/gml/2.1.2/feature.xsd");
    // wfs schema location
    flags->SetSchemaLocation(L"http://www.opengis.net/wfs", L"http://schemas.opengis.net/wfs/1.0.0/WFS-basic.xsd");
    // default namespace schema location
    flags->SetSchemaLocation(L"http://www.mynamespace.com/myns", L"http://www.mynamespace.com/myns/myns.xsd");

    // set the default namespace
    flags->SetDefaultNamespace(L"http://www.mynamespace.com/myns");

    FdoPtr<FdoXmlFeatureWriter> featureWriter = FdoXmlFeatureWriter::Create(propWriter, flags);

    FdoXmlFeatureSerializer::XmlSerialize(featureReader, featureWriter, flags);

#ifdef _WIN32
        // Compare output against expected results.
        UnitTestUtil::CheckOutput( "gml_write_master.txt", "gml_write.xml" );
#endif
 
	featureReader = new MyFeatureReader();
    xmlWriter = FdoXmlWriter::Create(L"gml_write2.xml", false);
    featureWriter = FdoXmlFeatureWriter::Create(xmlWriter, flags);

    FdoXmlFeatureSerializer::XmlSerialize(featureReader, featureWriter, flags);

#ifdef _WIN32
        // Compare output against expected results.
        UnitTestUtil::CheckOutput( "gml_write_master.txt", "gml_write2.xml" );
#endif
}





