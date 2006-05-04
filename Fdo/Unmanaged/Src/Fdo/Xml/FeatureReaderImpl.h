#ifndef FDO_XML_FEATUREREADER_IMPL_H_
#define FDO_XML_FEATUREREADER_IMPL_H_
//

//
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
//

#ifdef _WIN32
#pragma once
#endif

#include <Std.h>
#include <Fdo.h>
#include <Fdo/Xml/FeatureReader.h>

#include <Fdo/Xml/FeatureFlags.h>
#include <Fdo/Xml/FeatureContext.h>
#include <Fdo/Xml/SpatialContextFlags.h>

#include <Fdo/Xml/FeatureHandler.h>

#include <vector>

#include "BLOBProperty.h"

class FdoXmlDataProperty;
class FdoXmlGeometricProperty;
class FdoXmlAssociationProperty;
class FdoXmlGeometry;
class FdoXmlFeaturePropertyReader;

///<summary>
///</summary>
class FdoXmlFeatureReaderImpl : public FdoXmlFeatureReader
{
public:

	static FdoXmlFeatureReaderImpl * Create( 
		FdoXmlReader*                   reader,
    	FdoXmlFeatureFlags*      flags = NULL,
		int depth = 0
    );

    virtual FdoXmlFeaturePropertyReader* GetFeaturePropertyReader();


    virtual FdoFeatureSchemaCollection* GetFeatureSchemas();

    virtual void SetFeatureSchemas(FdoFeatureSchemaCollection* schemas);


	//////////////////////////////////////////////////////////////////////////////////////
	///   FdoIFeatureReader Overrides
	/////////////////////////////////////////////////////////////////////////////////////

    virtual FdoClassDefinition* GetClassDefinition();

    virtual FdoInt32 GetDepth();

    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);

    virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);


	
	//////////////////////////////////////////////////////////////////////////////////////
	///   FdoIReader Overrides
	/////////////////////////////////////////////////////////////////////////////////////

    virtual bool GetBoolean(FdoString* propertyName);

    virtual FdoByte GetByte(FdoString* propertyName);

	virtual FdoDateTime GetDateTime(FdoString* propertyName);

    virtual double GetDouble(FdoString* propertyName);

    virtual FdoInt16 GetInt16(FdoString* propertyName);

    virtual FdoInt32 GetInt32(FdoString* propertyName);

    virtual FdoInt64 GetInt64(FdoString* propertyName);

    virtual float GetSingle(FdoString* propertyName);

    virtual FdoString* GetString(FdoString* propertyName);

    virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    virtual bool IsNull(FdoString* propertyName);

	virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    virtual FdoIRaster* GetRaster(FdoString* propertyName);

    virtual bool ReadNext();

	virtual void Close();


	////////////////////////////////////////////////////////////////////////////////////////////
	///Feature Handler Overrides
	///////////////////////////////////////////////////////////////////////////////////////////
    virtual FdoXmlFeatureHandler* FeatureDocumentStart(FdoXmlFeatureContext*);

	virtual void FeatureDocumentEnd(FdoXmlFeatureContext*);

    virtual FdoXmlFeatureHandler* FeatureCollectionStart(FdoXmlFeatureContext*,FdoClassDefinition*);

    virtual FdoBoolean FeatureCollectionEnd(FdoXmlFeatureContext*);

    virtual FdoXmlFeatureHandler* FeatureStart(FdoXmlFeatureContext*,FdoClassDefinition*);

    virtual FdoBoolean FeatureEnd(FdoXmlFeatureContext*);

	virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoBoolean);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoByte);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDateTime);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDouble);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt16);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt32);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt64);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoFloat);

    virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoString*);

    virtual FdoBoolean FeatureStartRasterProperty(FdoXmlFeatureContext*, FdoString*, FdoIRaster*);

    virtual FdoBoolean FeatureEndRasterProperty(FdoXmlFeatureContext*);

    virtual FdoBoolean FeatureStartLobProperty(FdoXmlFeatureContext*, FdoString*);

    virtual FdoBoolean FeatureEndLobProperty(FdoXmlFeatureContext*);

    virtual FdoBoolean FeatureGeometricProperty(FdoXmlFeatureContext*, FdoString*, FdoByte*, FdoInt32);

    virtual FdoXmlFeatureHandler* FeatureStartObjectProperty(FdoXmlFeatureContext*, FdoString*, FdoClassDefinition*);

    virtual FdoBoolean FeatureEndObjectProperty(FdoXmlFeatureContext*);

    virtual FdoXmlFeatureHandler* FeatureStartAssociationProperty(FdoXmlFeatureContext*, FdoString*, FdoClassDefinition*);

    virtual FdoBoolean FeatureEndAssociationProperty(FdoXmlFeatureContext*);

    virtual FdoBoolean FeatureCharacters(FdoXmlFeatureContext*, FdoString*);

    virtual FdoBoolean FeatureBinaryData(FdoXmlFeatureContext*, FdoByte*, FdoSize);


protected:
	FdoXmlFeatureReaderImpl (){}
	FdoXmlFeatureReaderImpl(FdoXmlReader* reader, FdoXmlFeatureFlags* flags, int depth = 0);
    virtual ~FdoXmlFeatureReaderImpl (void);

    virtual void Dispose ()
    {
        delete this;
    }
	

	enum FeatureReaderType{
		FeatureReaderType_Unknown,
		FeatureReaderType_Feature,
		FeatureReaderType_FeatureCollection
	}m_featureReaderType;

	//feature collection properties 
	FdoXmlFeatureReaderImpl* GetCurrentFeatureForRead();
	void AddFeatureMember(FdoXmlFeatureReaderImpl* feature);

	//feature properties
	FdoString* GetDataProperty(FdoString* name);
    FdoLOBValue* GetBLOBProperty(FdoString* name);
	FdoXmlFeatureReaderImpl* GetAssociationProperty(FdoString* name);
	FdoXmlGeometry* GetGeometricProperty(FdoString* name);

	void AddDataProperty(FdoString* name, FdoString* value);
	void AddAssociationProperty(FdoString* name, FdoXmlFeatureReaderImpl* feature);
	void AddGeometricProperty(FdoString* name, FdoXmlGeometry* geometry);
	void AddGeometricProperty(FdoString* name, FdoByteArray* fgfByteArray);

private:

	FdoXmlReaderP m_xmlReader;
	int m_depth; //nested level of a feture collection
	bool m_incrementalParsing;

	//internal feature property reader that issues feature handler events
	FdoPtr<FdoXmlFeaturePropertyReader> m_featurePropertyReader;

	//feature collection
	int m_curFeatureIndex;
	std::vector<FdoXmlFeatureReaderImpl*> m_featureCollection;

	//feature properties
	std::vector<FdoXmlDataProperty*> m_dataProperties;
	std::vector<FdoXmlGeometricProperty*> m_geometricProperties;
	std::vector<FdoXmlAssociationProperty*> m_associationProperties;

    FdoPtr<FdoXmlBLOBPropertyCollection> m_blobProperties;
    FdoPtr<FdoIoMemoryStream> m_blobBuffer;
    FdoStringP m_blobPropertyName;

    FdoPtr<FdoFeatureSchemaCollection> m_schemas;
    FdoPtr<FdoXmlFeatureFlags> m_flags;
};

///<summary>FdoXmlSpatialContextReaderP is a FdoPtr on FdoXmlSpatialContextReader, provided for convenience.</summary>
typedef FdoPtr<FdoXmlFeatureReaderImpl> FdoXmlFeatureReaderImplP;

#endif

