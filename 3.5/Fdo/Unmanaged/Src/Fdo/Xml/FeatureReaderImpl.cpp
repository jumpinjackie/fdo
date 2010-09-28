/// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo.h>
#include "FeatureReaderImpl.h"
#include <Fdo/Xml/FeaturePropertyReader.h>


#include "GeometryHandler.h"
#include "Geometry.h"
#include "DataProperty.h"
#include "GeometricProperty.h"
#include "AssociationProperty.h"
#include <assert.h>

#include <iostream>
using namespace std;

FdoXmlFeatureReaderImpl::FdoXmlFeatureReaderImpl(FdoXmlReader* reader, FdoXmlFeatureFlags* flags, int depth)
{
	m_xmlReader = FDO_SAFE_ADDREF(reader);
    m_flags = FDO_SAFE_ADDREF(flags);
	m_depth = depth;

	//assume to be incremental parsing
	m_incrementalParsing = true;

	m_featurePropertyReader = NULL;
	m_curFeatureIndex = -1;
	m_featureReaderType = FeatureReaderType_Unknown;

    m_blobProperties = FdoXmlBLOBPropertyCollection::Create();

    m_propertyNames = FdoStringCollection::Create();
}

FdoXmlFeatureReaderImpl::~FdoXmlFeatureReaderImpl()
{
	while(!m_featureCollection.empty()){
		FDO_SAFE_RELEASE(m_featureCollection.back());
		m_featureCollection.pop_back();
	}

	while(!m_dataProperties.empty()){
		FDO_SAFE_RELEASE(m_dataProperties.back());
		m_dataProperties.pop_back();
	}

	while(!m_associationProperties.empty()){
		FDO_SAFE_RELEASE(m_associationProperties.back());
		m_associationProperties.pop_back();
	}

	while(!m_geometricProperties.empty()){
		FDO_SAFE_RELEASE(m_geometricProperties.back());
		m_geometricProperties.pop_back();
	}

}

FdoXmlFeatureReaderImpl * FdoXmlFeatureReaderImpl::Create(FdoXmlReader* reader, FdoXmlFeatureFlags* flags, int depth)
{
	return new FdoXmlFeatureReaderImpl(reader, flags, depth);
}

bool FdoXmlFeatureReaderImpl::ReadNext() 
{
	m_curFeatureIndex ++;

	if(m_incrementalParsing && m_depth == 0){

		//it is the top-level parser, create featurePropertyReader
        if(m_featurePropertyReader == NULL) {
			m_featurePropertyReader = FdoXmlFeaturePropertyReader::Create(m_xmlReader, m_flags);
            m_featurePropertyReader->SetFeatureSchemas(m_schemas);
        }
		
		m_featurePropertyReader->Parse(this, NULL, m_incrementalParsing);
	}

	int collSize = (int)m_featureCollection.size();
	return m_curFeatureIndex < collSize;
}

void FdoXmlFeatureReaderImpl::Close() 
{
}



FdoXmlFeaturePropertyReader* FdoXmlFeatureReaderImpl::GetFeaturePropertyReader()
{
	return NULL;
}


FdoFeatureSchemaCollection* FdoXmlFeatureReaderImpl::GetFeatureSchemas()
{
	return FDO_SAFE_ADDREF(m_schemas.p);
}

void FdoXmlFeatureReaderImpl::SetFeatureSchemas(FdoFeatureSchemaCollection* schemas)
{
    m_schemas = FDO_SAFE_ADDREF(schemas);
}

//FdoIFeatureReader overrides
FdoClassDefinition* FdoXmlFeatureReaderImpl::GetClassDefinition()
{
	return NULL;
}

FdoInt32 FdoXmlFeatureReaderImpl::GetDepth()
{
	return m_depth;
}

const FdoByte * FdoXmlFeatureReaderImpl::GetGeometry(FdoString* propertyName, FdoInt32 * count)
{
    FdoByte* data = NULL;
    FdoPtr<FdoByteArray> ba = this->GetGeometry(propertyName);
    if (NULL != ba.p)
    {
        data = ba->GetData();
        *count = ba->GetCount();
    }
    return data;
}

FdoByteArray* FdoXmlFeatureReaderImpl::GetGeometry(FdoString* propertyName)
{
	FdoPtr<FdoXmlGeometry> geometry = GetCurrentFeatureForRead()->GetGeometricProperty(propertyName);
	if(geometry != NULL)
		return geometry->GetFgf();

	return NULL;
}

FdoIFeatureReader* FdoXmlFeatureReaderImpl::GetFeatureObject(FdoString* propertyName)
{
	return GetCurrentFeatureForRead()->GetAssociationProperty(propertyName);
}

//FdoIReader overrides
bool FdoXmlFeatureReaderImpl::GetBoolean(FdoString* propertyName)
{
	FdoStringP rv(GetString(propertyName));
	return rv.ToBoolean();
}

FdoByte FdoXmlFeatureReaderImpl::GetByte(FdoString* propertyName)
{
	return 0;
}

FdoDateTime FdoXmlFeatureReaderImpl::GetDateTime(FdoString* propertyName)
{
	FdoStringP value = GetString (propertyName);	
	if (value.GetLength () == 0)
	{
		return FdoDateTime (0, 0, 0, 0, 0, 0);
	}

	FdoPtr<FdoDateTimeValue> dt;
	// let's try DATE first
	FdoStringP date = FdoStringP(L"DATE '") + value + L"'";
	try {
		dt = static_cast<FdoDateTimeValue *> (FdoDateTimeValue::Parse (date));
	} catch (FdoException* e) {
		e->Release();
		// oops, it's not a date, try TIMESTAMP
		FdoStringP timestamp = FdoStringP(L"TIMESTAMP '") + value + L"'";
		try {
			dt = static_cast<FdoDateTimeValue *> (FdoDateTimeValue::Parse (timestamp));
		} catch (FdoException* e1) {
			e1->Release();
			// failed again, try the last option: TIME, this time, do not catch any exception
			FdoStringP time = FdoStringP(L"TIME '") + value + L"'";
			dt = static_cast<FdoDateTimeValue *> (FdoDateTimeValue::Parse (time));
		}
	}

	return dt->GetDateTime ();
}

double FdoXmlFeatureReaderImpl::GetDouble(FdoString* propertyName) 
{
	return FdoStringP(GetString(propertyName)).ToDouble();
}

FdoInt16 FdoXmlFeatureReaderImpl::GetInt16(FdoString* propertyName) 
{
    FdoStringP rv(GetString(propertyName));
    return (FdoInt16)rv.ToLong();
}

FdoInt32 FdoXmlFeatureReaderImpl::GetInt32(FdoString* propertyName) 
{
    FdoStringP rv(GetString(propertyName));
    return (FdoInt32)rv.ToLong();
}

FdoInt64 FdoXmlFeatureReaderImpl::GetInt64(FdoString* propertyName) 
{
    FdoStringP rv(GetString(propertyName));
    return (FdoInt64)rv.ToLong();
}

float FdoXmlFeatureReaderImpl::GetSingle(FdoString* propertyName) 
{
    FdoStringP rv(GetString(propertyName));
    return (float)rv.ToDouble();
}

FdoString* FdoXmlFeatureReaderImpl::GetString(FdoString* propertyName) 
{
	return GetCurrentFeatureForRead()->GetDataProperty(propertyName);
}

FdoLOBValue* FdoXmlFeatureReaderImpl::GetLOB(FdoString* propertyName) 
{
    return GetCurrentFeatureForRead()->GetBLOBProperty(propertyName);
}

FdoIStreamReader* FdoXmlFeatureReaderImpl::GetLOBStreamReader(const wchar_t* propertyName ) 
{
	return NULL;
}

bool FdoXmlFeatureReaderImpl::IsNull(FdoString* propertyName) 
{
	// implementation of this method is important
	FdoXmlFeatureReaderImpl* current = GetCurrentFeatureForRead();
	if (current->GetDataProperty(propertyName) != NULL)
		return false;
	FdoPtr<FdoXmlGeometry> geometry = current->GetGeometricProperty(propertyName);
	if (geometry != NULL) {
		FdoPtr<FdoByteArray> geomBin = geometry->GetFgf();
		if (geomBin != NULL)
			return false;
		return true;
	}
	FdoPtr<FdoLOBValue> lobValue = current->GetBLOBProperty(propertyName);
	if (lobValue != NULL)
		return false;
	FdoPtr<FdoXmlFeatureReaderImpl> assoValue = current->GetAssociationProperty(propertyName);
	if (assoValue != NULL)
		return false;

	return true;
}

FdoIRaster* FdoXmlFeatureReaderImpl::GetRaster(FdoString* propertyName) 
{
	return NULL;
}

FdoInt32 FdoXmlFeatureReaderImpl::GetPropertyIndex(FdoString* propertyName)
{
    return m_propertyNames->IndexOf(propertyName, false);
}

FdoString* FdoXmlFeatureReaderImpl::GetPropertyName(FdoInt32 index)
{
    assert(index < m_propertyNames->GetCount());

    if (index < m_propertyNames->GetCount())
        return m_propertyNames->GetString(index);
    else
        return L"";
}

const FdoByte * FdoXmlFeatureReaderImpl::GetGeometry(FdoInt32 index, FdoInt32 * count)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry(propertyName, count);
}

FdoByteArray* FdoXmlFeatureReaderImpl::GetGeometry(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry(propertyName);
}

FdoIFeatureReader* FdoXmlFeatureReaderImpl::GetFeatureObject(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetFeatureObject(propertyName);
}

//FdoIReader overrides
bool FdoXmlFeatureReaderImpl::GetBoolean(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetBoolean(propertyName);
}

FdoByte FdoXmlFeatureReaderImpl::GetByte(FdoInt32 index)
{
    return 0;
}

FdoDateTime FdoXmlFeatureReaderImpl::GetDateTime(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDateTime(propertyName);
}

double FdoXmlFeatureReaderImpl::GetDouble(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDouble(propertyName);
}

FdoInt16 FdoXmlFeatureReaderImpl::GetInt16(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt16(propertyName);
}

FdoInt32 FdoXmlFeatureReaderImpl::GetInt32(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt32(propertyName);
}

FdoInt64 FdoXmlFeatureReaderImpl::GetInt64(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt64(propertyName);
}

float FdoXmlFeatureReaderImpl::GetSingle(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetSingle(propertyName);
}

FdoString* FdoXmlFeatureReaderImpl::GetString(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetString(propertyName);
}

FdoLOBValue* FdoXmlFeatureReaderImpl::GetLOB(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOB(propertyName);
}

FdoIStreamReader* FdoXmlFeatureReaderImpl::GetLOBStreamReader(FdoInt32 index) 
{
    return NULL;
}

bool FdoXmlFeatureReaderImpl::IsNull(FdoInt32 index) 
{
    FdoStringP propertyName = GetPropertyName(index);
    return IsNull(propertyName);
}

FdoIRaster* FdoXmlFeatureReaderImpl::GetRaster(FdoInt32 index) 
{
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////
///FdoXmlFeatureHandler overrides
/////////////////////////////////////////////////////////////////////////////////
FdoXmlFeatureHandler* FdoXmlFeatureReaderImpl::FeatureDocumentStart(FdoXmlFeatureContext*)
{
	return NULL;
}

void FdoXmlFeatureReaderImpl::FeatureDocumentEnd(FdoXmlFeatureContext*)
{
}


//Feature Collection
FdoXmlFeatureHandler* FdoXmlFeatureReaderImpl::FeatureCollectionStart(FdoXmlFeatureContext*,FdoClassDefinition*)
{
	//the first feature handler event determings whether it is a feature or a feature collection
	if(m_featureReaderType == FeatureReaderType_Unknown)
		m_featureReaderType = FeatureReaderType_FeatureCollection;

	return NULL;
}

FdoBoolean FdoXmlFeatureReaderImpl::FeatureCollectionEnd(FdoXmlFeatureContext*)
{
	return false;
}

//Feature
FdoXmlFeatureHandler* FdoXmlFeatureReaderImpl::FeatureStart(FdoXmlFeatureContext*,FdoClassDefinition*)
{
	//the first feature handler event determings whether it is a feature or a feature collection
	if(m_featureReaderType == FeatureReaderType_Unknown)
		m_featureReaderType = FeatureReaderType_Feature;

	if(m_featureReaderType == FeatureReaderType_FeatureCollection){

		FdoXmlFeatureReaderImplP nestedHandler = FdoXmlFeatureReaderImpl::Create(NULL, NULL, m_depth + 1);
		nestedHandler->m_featureReaderType = FeatureReaderType_Feature;

        AddFeatureMember(nestedHandler);

		return nestedHandler;
	}

	return NULL;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureEnd(FdoXmlFeatureContext*)
{
	return false;
}


//Association property
FdoXmlFeatureHandler* FdoXmlFeatureReaderImpl::FeatureStartAssociationProperty(FdoXmlFeatureContext*, FdoString* name, FdoClassDefinition*)
{
	FdoXmlFeatureReaderImplP nestedHandler;

	if(m_featureReaderType == FeatureReaderType_FeatureCollection){
		//association property in a feture collection must be a feature member,
		//it will be added into collection when the <feature> is encountered.
	}
	else{
		//association property in a feature is a collection storing the association objects as its members
		nestedHandler = GetAssociationProperty(name);
		if(nestedHandler == NULL){
			nestedHandler = FdoXmlFeatureReaderImpl::Create(NULL, NULL, m_depth + 1);
			nestedHandler->m_featureReaderType = FeatureReaderType_FeatureCollection;

			AddAssociationProperty(name, nestedHandler);
		}
	}

	return nestedHandler;
}

FdoBoolean FdoXmlFeatureReaderImpl::FeatureEndAssociationProperty(FdoXmlFeatureContext* context)
{
	bool isPauseParsing = false;

	if(m_incrementalParsing && m_depth == 0)
		isPauseParsing = true;

	return isPauseParsing;
}

//Object Property
FdoXmlFeatureHandler* FdoXmlFeatureReaderImpl::FeatureStartObjectProperty(FdoXmlFeatureContext*, FdoString* name, FdoClassDefinition*)
{
	//object property is by default a collection, 
	//the collection stores the nestedHandler as a member
	FdoXmlFeatureReaderImplP objectProperty = GetAssociationProperty(name);
	if(objectProperty == NULL){
		objectProperty = FdoXmlFeatureReaderImpl::Create(NULL, NULL, m_depth + 1);
		objectProperty->m_featureReaderType = FeatureReaderType_FeatureCollection;

		AddAssociationProperty(name, objectProperty);
	}
	
	//create feature reader to handle properties inside the object
	FdoXmlFeatureReaderImplP nestedHandler = FdoXmlFeatureReaderImpl::Create(NULL, NULL, m_depth + 2);
	nestedHandler->m_featureReaderType = FeatureReaderType_Feature;

	objectProperty->AddFeatureMember(nestedHandler);

	return nestedHandler;
}

FdoBoolean FdoXmlFeatureReaderImpl::FeatureEndObjectProperty(FdoXmlFeatureContext*)
{
	return false;
}


//Raster property
FdoBoolean FdoXmlFeatureReaderImpl::FeatureStartRasterProperty(FdoXmlFeatureContext*, FdoString*, FdoIRaster*)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureEndRasterProperty(FdoXmlFeatureContext*)
{
	return false;
}


//geometry property
FdoBoolean FdoXmlFeatureReaderImpl::FeatureGeometricProperty(FdoXmlFeatureContext*, FdoString* name, FdoByte* fgfBytes, FdoInt32 count)
{
    FdoPtr<FdoByteArray> byteArray;
    if (fgfBytes != NULL && count > 0) {
        byteArray = FdoByteArray::Create(fgfBytes, count);
    }

	AddGeometricProperty(name, byteArray);

	return false;

}


//Large Object Block property
FdoBoolean FdoXmlFeatureReaderImpl::FeatureStartLobProperty(FdoXmlFeatureContext*, FdoString* propName)
{
    m_blobPropertyName = propName;
    m_blobBuffer = FdoIoMemoryStream::Create();
	return false;
}



FdoBoolean FdoXmlFeatureReaderImpl::FeatureEndLobProperty(FdoXmlFeatureContext*)
{
    m_blobBuffer->Reset();
    FdoPtr<FdoByteArray> data = FdoByteArray::Create((FdoInt32)m_blobBuffer->GetLength());
	FdoSize szRead = m_blobBuffer->Read(data->GetData(), (FdoInt32)m_blobBuffer->GetLength());
	FdoByteArray::SetSize(data.p, (FdoInt32)szRead);
    FdoPtr<FdoBLOBValue> blobValue = FdoBLOBValue::Create(data);
    FdoPtr<FdoXmlBLOBProperty> blobProp = FdoXmlBLOBProperty::Create(m_blobPropertyName, blobValue);
    m_blobProperties->Add(blobProp);
    m_blobBuffer = NULL;
    m_blobPropertyName = L"";
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureCharacters(FdoXmlFeatureContext*, FdoString* propName)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureBinaryData(FdoXmlFeatureContext*, FdoByte* buffer, FdoSize count)
{
    m_blobBuffer->Write(buffer, count);
	return false;
}



//Feature Properties

FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoBoolean)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoByte)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDateTime)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDouble)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt16)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt32)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt64)
{
	return false;
}


FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoFloat)
{
	return false;
}



FdoBoolean FdoXmlFeatureReaderImpl::FeatureProperty(FdoXmlFeatureContext*, FdoString* name, FdoString* value)
{
	AddDataProperty(name, value);
	return false;
}


/////////////////////////////////////////////////////////////////////
///Container functions
//////////////////////////////////////////////////////////////////////
FdoXmlFeatureReaderImpl* FdoXmlFeatureReaderImpl::GetCurrentFeatureForRead()
{
	if(m_featureReaderType == FeatureReaderType_Feature){
		//this is just a feature, not a collection
		return this;
	}
	else{
		if(m_curFeatureIndex < 0)
			throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(SC_15_NEEDCALLREADNEXT),L"FdoXmlFeatureReader"));

		if(m_curFeatureIndex >= (int)m_featureCollection.size())
			throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(SC_16_INDEXOUTOFBOUND),L"FdoXmlFeatureReader"));

		FdoXmlFeatureReaderImpl* curFeature = m_featureCollection.at(m_curFeatureIndex);
		return curFeature;
	}
}


void FdoXmlFeatureReaderImpl::AddDataProperty(FdoString* name, FdoString* value)
{
	FdoXmlDataProperty* p = new FdoXmlDataProperty(name, value);
	m_dataProperties.push_back(p);

    m_propertyNames->Add(name);
}

void FdoXmlFeatureReaderImpl::AddGeometricProperty(FdoString* name, FdoXmlGeometry* geometry)
{
	FdoXmlGeometricProperty* p = new FdoXmlGeometricProperty(name, geometry);
	m_geometricProperties.push_back(p);

    m_propertyNames->Add(name);
}

void FdoXmlFeatureReaderImpl::AddGeometricProperty(FdoString* name, FdoByteArray* fgfByteArray)
{
	FdoXmlGeometricProperty* p = new FdoXmlGeometricProperty(name, fgfByteArray);
	m_geometricProperties.push_back(p);

    m_propertyNames->Add(name);
}

void FdoXmlFeatureReaderImpl::AddAssociationProperty(FdoString* name, FdoXmlFeatureReaderImpl* feature)
{
	FdoXmlAssociationProperty* p = new FdoXmlAssociationProperty(name, feature);
	m_associationProperties.push_back(p);

    m_propertyNames->Add(name);
}

FdoString* FdoXmlFeatureReaderImpl::GetDataProperty(FdoString* name)
{
	for(vector<FdoXmlDataProperty*>::iterator it = m_dataProperties.begin(); it != m_dataProperties.end(); it++){
		FdoXmlDataProperty* p = *it;
		if(wcscmp( name, p->GetName()) == 0)
			return p->GetValue();
	}

	return NULL;
}

FdoLOBValue* FdoXmlFeatureReaderImpl::GetBLOBProperty(FdoString* name)
{
    FdoPtr<FdoLOBValue> rv;
    FdoPtr<FdoXmlBLOBProperty> blobProp = m_blobProperties->FindItem(name);
    if (blobProp != NULL) {
        rv = blobProp->GetValue();
    }
    return FDO_SAFE_ADDREF(rv.p);
}


FdoXmlGeometry* FdoXmlFeatureReaderImpl::GetGeometricProperty(FdoString* name)
{
	for(vector<FdoXmlGeometricProperty*>::iterator it = m_geometricProperties.begin(); 
		it != m_geometricProperties.end(); 
		it++){

		FdoXmlGeometricProperty* p = *it;
		if(wcscmp( name, p->GetName()) == 0)
			return p->GetGeometry();
	}
	return NULL;
}

FdoXmlFeatureReaderImpl* FdoXmlFeatureReaderImpl::GetAssociationProperty(FdoString* name)
{
	for(vector<FdoXmlAssociationProperty*>::iterator it = m_associationProperties.begin(); 
		it != m_associationProperties.end(); 
		it++){

		FdoXmlAssociationProperty* p = *it;
		if(wcscmp( name, p->GetName()) == 0)
			return p->GetFeature();
	}
	return NULL;
}

void FdoXmlFeatureReaderImpl::AddFeatureMember(FdoXmlFeatureReaderImpl* feature)
{
	m_featureCollection.push_back(FDO_SAFE_ADDREF(feature));
}

