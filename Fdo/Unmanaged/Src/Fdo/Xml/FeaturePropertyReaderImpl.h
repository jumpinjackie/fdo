#ifndef FDO_XML_FEATUREPROPERTYREADER_IMPL_H_
#define FDO_XML_FEATUREPROPERTYREADER_IMPL_H_
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

#include <FdoStd.h>
#include <Fdo/Xml/FeaturePropertyReader.h>

#include <vector>

class FdoXmlFeatureContext;
class FdoXmlFeatureFlags;
class FdoXmlFeatureHandler;
class FdoXmlGeometryHandler;
class FdoXmlSchemaManager;
class FdoXmlLpClassDefinition;

class FdoXmlFeaturePropertyReaderImpl : public FdoXmlFeaturePropertyReader
{
public:

    virtual void Dispose ()
    {
        delete this;
    }

	static FdoXmlFeaturePropertyReaderImpl * Create( 
		FdoXmlReader*               reader,
    	FdoXmlFeatureFlags*         flags = NULL
    );

    virtual FdoXmlReader* GetXmlReader();
    virtual FdoFeatureSchemaCollection* GetFeatureSchemas();
    virtual void SetFeatureSchemas(FdoFeatureSchemaCollection* schemas);
    virtual FdoBoolean Parse(
        FdoXmlFeatureHandler* featureHandler = NULL, 
        FdoXmlFeatureContext* featureContext = NULL, 
        FdoBoolean incremental = false
    );

    virtual FdoBoolean GetEOF();

    virtual FdoXmlSaxHandler* XmlStartDocument(FdoXmlSaxContext*);

    virtual void XmlEndDocument(FdoXmlSaxContext*);

    virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext*, FdoString*, FdoString* name, FdoString*, FdoXmlAttributeCollection*);

    virtual FdoBoolean XmlEndElement(FdoXmlSaxContext*, FdoString*, FdoString*, FdoString*);

    virtual void XmlCharacters(FdoXmlSaxContext*, FdoString*);



protected:
	FdoXmlFeaturePropertyReaderImpl();
	virtual ~FdoXmlFeaturePropertyReaderImpl();
	FdoXmlFeaturePropertyReaderImpl(FdoXmlReader* reader, FdoXmlFeatureFlags* flags);


private:
	//state machine helpers
	enum GmlBaseType{
		GmlBaseType_FeatureCollection,
		GmlBaseType_FeatureAssociation,
		GmlBaseType_Feature,
		GmlBaseType_GeometryAssociation,
		GmlBaseType_BoundingShape,
		GmlBaseType_SimpleValue,
		GmlBaseType_GenericComplexType,
		GmlBaseType_Unknown,
        GmlBaseType_Pending,
        GmlBaseType_hexBinary,
        GmlBaseType_base64Binary,
		GmlBaseType_GmlGeometryAssociation,
		GmlBaseType_GmlDirectGeometry
	};

	enum ParsingState{
		ParsingState_Document,
		ParsingState_FeatureCollection,
		ParsingState_FeatureAssociation,
		ParsingState_Feature,
		ParsingState_GeometryAssociation,
		ParsingState_BoundingShape,
		ParsingState_SimpleValue,
		ParsingState_GenericComplexType,
		ParsingState_Unknown,
        ParsingState_ElementPending,
        ParsingState_hexBinary,
        ParsingState_base64Binary,
		ParsingState_GmlGeometryAssociation,
        ParsingState_GmlDirectGeometry
	};


	std::vector<ParsingState> m_parsingStateStack;
    //std::vector<FdoXmlElementMappingCollection*> m_elementsStack;
    std::vector<FdoXmlLpClassDefinition*> m_lpClassStack;
    FdoStringP m_pendingElementName;
    FdoStringP m_pendingElementUri;



	FdoXmlReaderP m_xmlReader;
	FdoPtr<FdoXmlFeatureFlags> m_flags;
	FdoPtr<FdoXmlFeatureContext> m_featureContext;

	std::vector<FdoXmlFeatureHandler*> m_featureHandlerStack;

	bool m_incremental;

	FdoStringP m_dataProperty;
	FdoPtr<FdoXmlGeometryHandler> m_geometryHandler;

    FdoPtr<FdoFeatureSchemaCollection> m_schemas;
    FdoPtr<FdoXmlSchemaManager> m_schemaManager;
    
// 
// helpers
//
private:
	GmlBaseType getGmlBaseType(FdoString* elementName, FdoString* elementUri);
    bool isTypeOf(FdoString* elementName, FdoString* elementUri, FdoString* type, bool bCaseSens = true );
    FdoXmlLpClassDefinition* getClassDef(FdoString* name, FdoString* uri);
    int m_activeGmlGeometryType;
};

///<summary>FdoXmlFeaturePropertyReaderP is a FdoPtr on FdoXmlFeaturePropertyReaderImpl, provided for convenience.</summary>
typedef FdoPtr<FdoXmlFeaturePropertyReaderImpl> FdoXmlFeaturePropertytReaderImplP;


#endif

