// 
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

#include <Fdo.h>
#include "FeaturePropertyReaderImpl.h"

#include <Fdo/Xml/FeatureFlags.h>
#include <Fdo/Xml/FeatureContext.h>
#include <Fdo/Xml/FeatureReader.h>

#include "GeometryHandler.h"
#include "Geometry.h"
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>

// logical physical schema items
#include "SchemaManager.h"
#include "LpSchema.h"
#include "LpClassDefinition.h"
#include "LpPropertyDefinition.h"
#include "LpGmlElementDefinition.h"


using namespace std;
FdoXmlFeaturePropertyReaderImpl::FdoXmlFeaturePropertyReaderImpl()
{
}

FdoXmlFeaturePropertyReaderImpl::~FdoXmlFeaturePropertyReaderImpl()
{
}

FdoXmlFeaturePropertyReaderImpl::FdoXmlFeaturePropertyReaderImpl(FdoXmlReader* reader, FdoXmlFeatureFlags* flags)
{
	m_xmlReader = FDO_SAFE_ADDREF(reader);
	m_flags = FDO_SAFE_ADDREF(flags);
}

FdoXmlFeaturePropertyReaderImpl * FdoXmlFeaturePropertyReaderImpl::Create(FdoXmlReader* reader, FdoXmlFeatureFlags* flags)
{
	return new FdoXmlFeaturePropertyReaderImpl(reader, flags);
}

FdoXmlReader* FdoXmlFeaturePropertyReaderImpl::GetXmlReader()
{
	return FDO_SAFE_ADDREF((FdoXmlReader*)m_xmlReader);
}

FdoFeatureSchemaCollection* FdoXmlFeaturePropertyReaderImpl::GetFeatureSchemas()
{
	return FDO_SAFE_ADDREF(m_schemas.p);
}

void FdoXmlFeaturePropertyReaderImpl::SetFeatureSchemas(FdoFeatureSchemaCollection* schemas)
{
    m_schemas = FDO_SAFE_ADDREF(schemas);
    m_schemaManager = NULL;
    if (m_schemas != NULL && m_flags != NULL) {
        FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = m_flags->GetSchemaMappings();
        if (schemaMappings != NULL) {
            m_schemaManager = FdoXmlSchemaManager::Create(m_schemas, m_flags);
        }
    }
}

FdoBoolean FdoXmlFeaturePropertyReaderImpl::Parse(FdoXmlFeatureHandler* featureHandler, FdoXmlFeatureContext* featureContext, FdoBoolean incremental)
{
	m_featureContext = FDO_SAFE_ADDREF(featureContext);
	m_incremental = incremental;

	m_featureHandlerStack.push_back(featureHandler);

	m_xmlReader->Parse(this, NULL, m_incremental);
	return false;
}

FdoBoolean FdoXmlFeaturePropertyReaderImpl::GetEOF()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/// FdoXmlSaxHandler
/////////////////////////////////////////////////////////////////////////////////////
FdoXmlSaxHandler* FdoXmlFeaturePropertyReaderImpl::XmlStartDocument(FdoXmlSaxContext* context)
{
	//indicating start of global elements
	m_parsingStateStack.push_back(ParsingState_Document);
    m_lpClassStack.push_back(NULL);

	FdoXmlFeatureHandler* curFeatureHandler = m_featureHandlerStack.back();

	FdoXmlFeatureHandler* nextFeatureHandler = NULL;
	nextFeatureHandler = curFeatureHandler->FeatureDocumentStart(m_featureContext);

	//handle feature handler stack
	if(nextFeatureHandler != NULL)
		m_featureHandlerStack.push_back(nextFeatureHandler);
	else
		m_featureHandlerStack.push_back(curFeatureHandler);


    return NULL;
}

void FdoXmlFeaturePropertyReaderImpl::XmlEndDocument(FdoXmlSaxContext* context)
{
	m_parsingStateStack.pop_back();
    m_lpClassStack.pop_back();

	m_featureHandlerStack.pop_back();
	FdoXmlFeatureHandler* curFeatureHandler = m_featureHandlerStack.back();

	curFeatureHandler->FeatureDocumentEnd(m_featureContext);
}

FdoXmlLpClassDefinition* FdoXmlFeaturePropertyReaderImpl::getClassDef(FdoString* name, FdoString* uri)
{
    FdoPtr<FdoXmlLpClassDefinition> rv;
    FdoXmlLpClassDefinition* classDef = m_lpClassStack.back();
    if (classDef == NULL) {
        if (m_schemaManager != NULL) {
            FdoPtr<FdoXmlLpSchema> schema = m_schemaManager->UriToSchema(uri);
            if (schema != NULL) {
                FdoPtr<FdoXmlLpGmlElementDefinition> lpGmlElement = schema->ElementFromGmlName(uri, name);
                if (lpGmlElement != NULL) {
                    rv = lpGmlElement->GetClassDefinition();
                }
            }
        }
    } else {
        FdoPtr<FdoXmlLpPropertyDefinition> prop = classDef->PropertyFromGml(uri, name);
        if (prop != NULL) {
            FdoPtr<FdoXmlElementMapping> elementMapping = prop->GetElementMapping();
            if (elementMapping != NULL) {
                FdoPtr<FdoXmlClassMapping> classMapping = elementMapping->GetClassMapping();
                if (classMapping != NULL) {
                    FdoPtr<FdoXmlLpSchema> schema = m_schemaManager->UriToSchema(uri);
                    if (schema != NULL) {
                        rv = schema->ClassFromName(classMapping->GetName());
                    }
                }
            }
        }

    }
    return FDO_SAFE_ADDREF(rv.p);
}

FdoXmlSaxHandler* FdoXmlFeaturePropertyReaderImpl::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts)
{
	ParsingState curState = m_parsingStateStack.back();
    // first we must check if there is any pending element that was not handled
    if (curState == ParsingState_ElementPending) {
        // current pending element has subelement, indicating that it is an object property
        // first we must restore the state to the time just before this pending element added
        m_parsingStateStack.pop_back();
        m_featureHandlerStack.pop_back();
		m_parsingStateStack.push_back(ParsingState_GenericComplexType);
        FdoXmlFeatureHandler* curFeatureHandler = m_featureHandlerStack.back();

        // send out this delayed event
		FdoXmlFeatureHandler* nextFeatureHandler = curFeatureHandler->FeatureStartObjectProperty(m_featureContext, m_pendingElementName, NULL);
	    if(nextFeatureHandler != NULL)
		    m_featureHandlerStack.push_back(nextFeatureHandler);
	    else
		    m_featureHandlerStack.push_back(curFeatureHandler);
        
        // then set current state
        curState = m_parsingStateStack.back();
    }

	FdoXmlFeatureHandler* curFeatureHandler = m_featureHandlerStack.back();
	FdoXmlSaxHandler* nextSaxHandler = NULL;
	FdoXmlFeatureHandler* nextFeatureHandler = NULL;

	GmlBaseType baseType = getGmlBaseType(name, uri);

	switch(baseType){
		//feature collection
		case GmlBaseType_FeatureCollection:
			m_parsingStateStack.push_back(ParsingState_FeatureCollection);

			//TODO: get class definition
			nextFeatureHandler = curFeatureHandler->FeatureCollectionStart(m_featureContext, NULL);

			break;

		//feature association
		case GmlBaseType_FeatureAssociation:
			
			m_parsingStateStack.push_back(ParsingState_FeatureAssociation);
			
			//TODO: get class definition
			nextFeatureHandler = curFeatureHandler->FeatureStartAssociationProperty(m_featureContext, name, NULL);

			break;

		//feature
		case GmlBaseType_Feature:
            {

                // find out current complex type's class definition
                FdoPtr<FdoXmlLpClassDefinition> classDef = getClassDef(name, uri);
                if (classDef != NULL)
                    m_lpClassStack.push_back(classDef.p);

			    m_parsingStateStack.push_back(ParsingState_Feature);

			    //TODO: get class definition
			    nextFeatureHandler = curFeatureHandler->FeatureStart(m_featureContext, NULL);
			    break;
            }

		//gml geometry association
		case GmlBaseType_GmlGeometryAssociation:
			m_parsingStateStack.push_back(ParsingState_GmlGeometryAssociation);
			
			m_geometryHandler = FdoXmlGeometryHandler::Create();
			nextSaxHandler = m_geometryHandler;
			break;
		//geometry association
		case GmlBaseType_GeometryAssociation:

			m_parsingStateStack.push_back(ParsingState_GeometryAssociation);
			
			m_geometryHandler = FdoXmlGeometryHandler::Create();
			nextSaxHandler = m_geometryHandler;
			break;

		//bounding shape
		case GmlBaseType_BoundingShape:


			m_parsingStateStack.push_back(ParsingState_BoundingShape);

			m_geometryHandler = FdoXmlGeometryHandler::Create();
			nextSaxHandler = m_geometryHandler;

			break;

		//generic complex type
		case GmlBaseType_GenericComplexType:
            {
                // find out current complex type's class definition
                FdoPtr<FdoXmlLpClassDefinition> classDef = getClassDef(name, uri);
                if (classDef != NULL)
                    m_lpClassStack.push_back(classDef.p);
    			
			    m_parsingStateStack.push_back(ParsingState_GenericComplexType);
    			
			    //TODO: get class definition
			    nextFeatureHandler = curFeatureHandler->FeatureStartObjectProperty(m_featureContext, name, NULL);

			    break;
            }
		//simple value
		case GmlBaseType_SimpleValue:
			m_parsingStateStack.push_back(ParsingState_SimpleValue);
			m_dataProperty = L"";
			break;
        
        // element needs to be pended as we can not determinte the element type until next element reached
        // or next element end reached
        case GmlBaseType_Pending:
            m_parsingStateStack.push_back(ParsingState_ElementPending);
            m_dataProperty = L"";
            break;
        case GmlBaseType_hexBinary:
            m_parsingStateStack.push_back(ParsingState_hexBinary);
            curFeatureHandler->FeatureStartLobProperty(m_featureContext, name);
            m_dataProperty = L"";
            break;
        case GmlBaseType_base64Binary:
            m_parsingStateStack.push_back(ParsingState_base64Binary);
            curFeatureHandler->FeatureStartLobProperty(m_featureContext, name);
            m_dataProperty = L"";
            break;
		default:
		    m_parsingStateStack.push_back(ParsingState_Unknown);
			break;
	}

	//handle feature handler stack
	if(nextFeatureHandler != NULL)
		m_featureHandlerStack.push_back(nextFeatureHandler);
	else
		m_featureHandlerStack.push_back(curFeatureHandler);

	return nextSaxHandler;
}


class HexBin
{
public:
    static FdoInt32 decode(const char* input, FdoInt32 count, FdoByte* decodedBuffer) {
        int writePt = 0;
        int readPt = 0;
        FdoByte current = 0;
        while (readPt < count) {
            char c = input[readPt];
            FdoByte tmp;
            if (c >= '0' && c <= '9')
                tmp = c - '0';
            else if (c >= 'a' && c <= 'f')
                tmp = 10 + (c - 'a');
            else if (c >= 'A' && c <= 'F')
                tmp = 10 + (c - 'A');
            else
                return -1;
            if (readPt % 2 == 0) 
                current = tmp << 4;
            else {
                current |= tmp;
                decodedBuffer[writePt++] = current;
            }
            readPt++;
        }
        if (readPt % 2 != 0)
            decodedBuffer[writePt++] = current;

        return writePt;        
    }
};



FdoBoolean FdoXmlFeaturePropertyReaderImpl::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
	bool isPauseParsing = false;

	m_featureHandlerStack.pop_back();
	FdoXmlFeatureHandler* curFeatureHandler = m_featureHandlerStack.back();

	ParsingState curState = m_parsingStateStack.back();
	m_parsingStateStack.pop_back();

	ParsingState prevState = ParsingState_Unknown;

	FdoXmlGeometry* tempGeometry = NULL;
	FdoByteArray* tempByteArray = NULL;

	switch(curState)
	{
	//feature collection
	case ParsingState_FeatureCollection:
		isPauseParsing = curFeatureHandler->FeatureCollectionEnd(m_featureContext);
		break;

	//feature association
	case ParsingState_FeatureAssociation:
		isPauseParsing = curFeatureHandler->FeatureEndAssociationProperty(m_featureContext);

		break;

	//feature
	case ParsingState_Feature:
		
        if (m_lpClassStack.back() != NULL)
            m_lpClassStack.pop_back();

		isPauseParsing = curFeatureHandler->FeatureEnd(m_featureContext);
		break;

	//bounding shape
	case ParsingState_BoundingShape:

		//"Bounds" is FDO property for gml:boundedBy
		tempGeometry =  m_geometryHandler->GetGeometry();
		if (tempGeometry)
		{
			tempByteArray = tempGeometry->GetFgf();

			isPauseParsing = curFeatureHandler->FeatureGeometricProperty(m_featureContext, 
				L"Bounds", 
				tempByteArray->GetData(), 
				tempByteArray->GetCount());
		}		

		FDO_SAFE_RELEASE(tempGeometry);
		FDO_SAFE_RELEASE(tempByteArray);

		break;

	//gml geometry association
	case ParsingState_GmlGeometryAssociation:
    {
		tempGeometry =  m_geometryHandler->GetGeometry();
		tempByteArray = tempGeometry->GetFgf();
        FdoStringP pPropName = name;
        if (NULL != m_lpClassStack.back())
        {
            FdoXmlLpClassDefinition* classDef = m_lpClassStack.back();
            FdoString* pBaseName = classDef->PropertyMappingNameFromGmlAlias(name);
            if (pBaseName != NULL)
                pPropName = pBaseName;
            else
            {
                pPropName = L"gml/";
                pPropName += name;
            }
        }
        if (tempByteArray != NULL)
        {
            isPauseParsing = curFeatureHandler->FeatureGeometricProperty(m_featureContext, 
			    pPropName,
			    tempByteArray->GetData(), 
			    tempByteArray->GetCount());
        } else
        {
		    isPauseParsing = curFeatureHandler->FeatureGeometricProperty(m_featureContext, 
			    pPropName,
			    NULL, 
			    0);
        }

		FDO_SAFE_RELEASE(tempGeometry);
		FDO_SAFE_RELEASE(tempByteArray);
    }
		break;

	//geometry association
	case ParsingState_GeometryAssociation:
    {
		tempGeometry =  m_geometryHandler->GetGeometry();
		tempByteArray = tempGeometry->GetFgf();
        FdoStringP pPropName = name;
        if (NULL != m_lpClassStack.back())
        {
            FdoXmlLpClassDefinition* classDef = m_lpClassStack.back();
            FdoString* pBaseName = classDef->PropertyMappingNameFromGmlAlias(name);
            if (pBaseName != NULL)
                pPropName = pBaseName;
        }
        if (tempByteArray != NULL) {
		    isPauseParsing = curFeatureHandler->FeatureGeometricProperty(m_featureContext, 
			    pPropName,
			    tempByteArray->GetData(), 
			    tempByteArray->GetCount());
        } else {
		    isPauseParsing = curFeatureHandler->FeatureGeometricProperty(m_featureContext, 
			    pPropName,
			    NULL, 
			    0);
        }

		FDO_SAFE_RELEASE(tempGeometry);
		FDO_SAFE_RELEASE(tempByteArray);
    }
		break;

	//Generic Complex Type
	case ParsingState_GenericComplexType:
        
        if (m_lpClassStack.back() != NULL)
            m_lpClassStack.pop_back();

		isPauseParsing = curFeatureHandler->FeatureEndObjectProperty(m_featureContext);

		break;

	//simple value & element pending
	case ParsingState_SimpleValue:
    case ParsingState_ElementPending:
		isPauseParsing = curFeatureHandler->FeatureProperty(m_featureContext, name, (FdoString*)m_dataProperty);
		break;

    case ParsingState_hexBinary:
        {
            FdoInt32 count = (FdoInt32)m_dataProperty.GetLength();
            FdoByte* decoded = new FdoByte[count / 2 + 1];
            FdoInt32 decodedLen = HexBin::decode((const char*)m_dataProperty, count, decoded);
            if (decodedLen > 0)
                curFeatureHandler->FeatureBinaryData(m_featureContext, decoded, decodedLen);
            delete[] decoded;
            curFeatureHandler->FeatureEndLobProperty(m_featureContext);

        }
        break;
    case ParsingState_base64Binary:
        {
            FdoSize len;
            XMLByte* decoded = XERCES_CPP_NAMESPACE::Base64::decode((const XMLByte*)(const char*)m_dataProperty, &len);
            if (decoded != NULL) {
                curFeatureHandler->FeatureBinaryData(m_featureContext, decoded, len);
                XERCES_CPP_NAMESPACE::XMLString::release(&decoded);
            }
            curFeatureHandler->FeatureEndLobProperty(m_featureContext);
        }
        break;

	default:
		break;
	}

	return isPauseParsing;
}

void FdoXmlFeaturePropertyReaderImpl::XmlCharacters(FdoXmlSaxContext* context, FdoString* characters)
{
	if(!m_parsingStateStack.empty()){
		ParsingState curState = m_parsingStateStack.back();

		if(curState == ParsingState_SimpleValue ||
            curState == ParsingState_ElementPending ||
            curState == ParsingState_hexBinary ||
            curState == ParsingState_base64Binary)
			m_dataProperty += characters;
	}
}


/////////////////////////////////////////////////////////////////////////////////////
///Helper functions
////////////////////////////////////////////////////////////////////////////////////


bool FdoXmlFeaturePropertyReaderImpl::isTypeOf(FdoString* elementName, FdoString* elementUri, FdoString* type, bool bCaseSens)
{
    bool rv = false;
    if (m_schemaManager != NULL) {
        FdoPtr<FdoXmlLpSchema> schema = m_schemaManager->UriToSchema(elementUri);
        if (schema != NULL) {
            FdoPtr<FdoXmlLpGmlElementDefinition> element = schema->ElementFromGmlName(elementUri, elementName, bCaseSens);
            if (element != NULL) {
                FdoPtr<FdoXmlLpClassDefinition> classDef = element->GetClassDefinition();
                if (classDef != NULL) {
                    FdoPtr<FdoXmlClassMapping> classMapping = classDef->GetClassMapping();
                    if (classMapping != NULL && wcscmp(classMapping->GetWkBaseName(), type) == 0) 
                        rv = true;
                }
            }
        }
    }
    return rv;
}


//The hard-coded implementation here will be replaced by 
//schema support in the future
FdoXmlFeaturePropertyReaderImpl::GmlBaseType FdoXmlFeaturePropertyReaderImpl::getGmlBaseType(FdoString* elementName, FdoString* elementUri)
{
	ParsingState curState = m_parsingStateStack.back();
    GmlBaseType rv = GmlBaseType_Unknown;
    switch (curState) {
        case ParsingState_Document:
            {
                // first check the predefined elements
                // then find in the schema mappings
                if ((wcscmp(elementUri, L"http://www.opengis.net/wfs") == 0 &&
                    wcscmp(elementName, L"FeatureCollection") == 0) ||
                    isTypeOf(elementName, elementUri, FdoGml212::mAbstractFeatureCollection, false))
                    rv = GmlBaseType_FeatureCollection;
                break;
            }
        case ParsingState_FeatureCollection:
            {
                // similar as above
                if ((wcscmp(elementUri, FdoXml::mGmlUri) == 0 &&
                    wcscmp(elementName, L"featureMember") == 0) ||
                    isTypeOf(elementName, elementUri, FdoGml212::mFeatureAssociation, false))
                    rv = GmlBaseType_FeatureAssociation;
                else if (wcscmp(elementUri, FdoXml::mGmlUri) == 0 &&
                    wcscmp(elementName, L"boundedBy") == 0)
                    rv = GmlBaseType_BoundingShape;

                break;
            }
        case ParsingState_FeatureAssociation:
            {
                // similar as above except that there is no predefined feature element
                if (isTypeOf(elementName, elementUri, FdoGml212::mAbstractFeature))
                    rv = GmlBaseType_Feature;
                else if (m_schemaManager == NULL) // do best effort, assume that it is a feature
                    rv = GmlBaseType_Feature;
                break;
            }
        case ParsingState_Feature:
        case ParsingState_GenericComplexType:
            {
                // first try gml well known properties
                if (wcscmp(elementUri, FdoXml::mGmlUri) == 0) {
                    if (wcscmp(elementName, L"boundedBy") == 0)
                        rv = GmlBaseType_BoundingShape;
                    else if (wcscmp(elementName, L"pointProperty") == 0 ||
                                wcscmp(elementName, L"polygonProperty") == 0 ||
                                wcscmp(elementName, L"lineStringProperty") == 0 ||
                                wcscmp(elementName, L"multiPointProperty") == 0 ||
                                wcscmp(elementName, L"multiLineStringProperty") == 0 ||
                                wcscmp(elementName, L"multiPolygonProperty") == 0 ||
                                wcscmp(elementName, L"multiGeometryProperty") == 0 ||
                                wcscmp(elementName, L"geometryProperty") == 0 ||
                                wcscmp(elementName, L"location") == 0 ||
                                wcscmp(elementName, L"centerOf") == 0 ||
                                wcscmp(elementName, L"position") == 0 ||
                                wcscmp(elementName, L"extentOf") == 0 ||
                                wcscmp(elementName, L"coverage") == 0 ||
                                wcscmp(elementName, L"edgeOf") == 0 ||
                                wcscmp(elementName, L"centerLineOf") == 0 ||
                                wcscmp(elementName, L"multiLocation") == 0 ||
                                wcscmp(elementName, L"multiCenterOf") == 0 ||
                                wcscmp(elementName, L"multiPosition") == 0 ||
                                wcscmp(elementName, L"multiCenterLineOf") == 0 ||
                                wcscmp(elementName, L"multiEdgeOf") == 0 ||
                                wcscmp(elementName, L"multiCoverage") == 0 ||
                                wcscmp(elementName, L"multiExtentOf") == 0)
                        rv = GmlBaseType_GmlGeometryAssociation;

                }
                if (rv != GmlBaseType_Unknown)
                    break;
                if (m_lpClassStack.back() != NULL) { // it is a property

                    FdoXmlLpClassDefinition* classDef = m_lpClassStack.back();
                    FdoPtr<FdoXmlLpPropertyDefinition> prop = classDef->PropertyFromGml(elementUri, elementName);
                    if (prop != NULL) {
                        FdoPtr<FdoXmlElementMapping> element = prop->GetElementMapping();
                        if (element != NULL) {
                            FdoPtr<FdoXmlClassMapping> elementClass = element->GetClassMapping();
                            if (elementClass != NULL) { // it is an object or feature association or geometry association
                                FdoString* wkBaseName = elementClass->GetWkBaseName();
                                if (wcscmp(wkBaseName, FdoGml212::mPointProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mPolygonProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mLineStringProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mMultiPointProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mMultiPolygonProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mMultiLineStringProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mMultiGeometryProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mGeometryAssociation) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mGeometryProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mAbstractGeometry) == 0) // geometry association
                                    rv = GmlBaseType_GeometryAssociation;
                                else if (wcscmp(wkBaseName, FdoGml212::mFeatureAssociation) == 0) // feature association
                                    rv = GmlBaseType_FeatureAssociation;
                                else // object property
                                    rv = GmlBaseType_GenericComplexType;
                            } else { // it is data property
                                // find out whether it is a LOB property
                                FdoString* className = element->GetClassName();
                                if (wcscmp(className, L"hexBinary") == 0)
                                    rv = GmlBaseType_hexBinary;
                                else if (wcscmp(className, L"base64Binary") == 0)
                                    rv = GmlBaseType_base64Binary;
                                else
                                    rv = GmlBaseType_SimpleValue;
                            }
                        }
                    }
                } else { // try the "best offort if the schema mappings are not available
                    m_pendingElementName = elementName;
                    m_pendingElementUri = elementUri;
                    rv = GmlBaseType_Pending;
                }
            }
            break;
        default:
            break;
    }

    return rv;
}
