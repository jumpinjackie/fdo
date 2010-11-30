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
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#ifdef UNICODE
#define GetClassNameW  GetClassName
#else
#define GetClassNameA  GetClassName
#endif // !UNICODE
#endif

#include <Fdo.h>
#include "FeaturePropertyReaderImpl.h"

#include <Fdo/Xml/FeatureFlags.h>
#include <Fdo/Xml/FeatureContext.h>
#include <Fdo/Xml/FeatureReader.h>

#include "GeometryHandler.h"
#include "Geometry.h"
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// logical physical schema items
#include "SchemaManager.h"
#include "LpSchema.h"
#include "LpClassDefinition.h"
#include "LpPropertyDefinition.h"
#include "LpGmlElementDefinition.h"


using namespace std;
FdoXmlFeaturePropertyReaderImpl::FdoXmlFeaturePropertyReaderImpl()
{
    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Unknown;
}

FdoXmlFeaturePropertyReaderImpl::~FdoXmlFeaturePropertyReaderImpl()
{
}

FdoXmlFeaturePropertyReaderImpl::FdoXmlFeaturePropertyReaderImpl(FdoXmlReader* reader, FdoXmlFeatureFlags* flags)
{
	m_xmlReader = FDO_SAFE_ADDREF(reader);
	m_flags = FDO_SAFE_ADDREF(flags);
    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Unknown;
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
        if ( schemaMappings == NULL ) 
            schemaMappings = schemas->GetXmlSchemaMappings();

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
    FdoStringP validName = name;
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

	////////////////////////////////////////////////////////////////////////////////////
    // Greg Boone. October 1, 2009.
    // This is a bit of a workaround to get schemas from MapGuide 2009 to parse correctly.
    // I found that the GetFeature responses from MapGuide were not returning any URI
    // Information. Unfortunately this caused the entire parse to fail as the parsing
    // of Features is tied off of the URI identifier for schema and class retrieval.
    // In an attempt to enable continued parsing, the following block of code was
    // added to search through the available schemas to identify the schema name based
    // on the use of the 'name' element being parsed being the class name. This does
    // work in limited scenarios. There is the chance for failure if the same class
    // exists in multiple schemas. If multiple schemas are matched to a class name
    // the workaround will be abandoned since it will not be possible to determine 
    // which schema the class actually conforms with.
	////////////////////////////////////////////////////////////////////////////////////
    FdoStringP lUri = uri;
    if (curState == ParsingState_FeatureAssociation && lUri == L"" && NULL != m_schemaManager) {
        int schemaCount = 0;
        FdoPtr<FdoFeatureSchema> theSchema;
        FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = m_flags->GetSchemaMappings();
        if (NULL != schemaMappings) {
            FdoPtr<FdoFeatureSchemaCollection> coll = GetFeatureSchemas();
            for (int i =0; i<coll->GetCount(); i++) {
                FdoPtr<FdoFeatureSchema> schema = coll->GetItem(i);
                FdoPtr<FdoClassCollection> classes = schema->GetClasses();
                FdoPtr<FdoClassDefinition> cls = classes->FindItem(name);
                if (NULL != cls) {
                    schemaCount++;
                    theSchema = schema;
                }
            }
        }
        if (NULL != theSchema && schemaCount == 1) {
            FdoPtr<FdoXmlLpSchema> lpSchema = m_schemaManager->NameToSchema(theSchema->GetName());
            if (NULL != lpSchema) {
                FdoPtr<FdoXmlSchemaMapping> schemaMappings = lpSchema->GetMappings();
                if (NULL != schemaMappings) {
                    // Set the parser's feature URI to that of the schema mapping's
                    // target namespace. This will be used later when parsing feature
                    // sub-elements.
                    m_featureURI = lUri = schemaMappings->GetTargetNamespace();
                }
            }
        }
    }
    // Once feature parsing starts, we also have to use the feature URI for parsing the
    // underlying elements of the feature
    else if (curState == ParsingState_Feature && lUri == L"")
    {
        lUri = m_featureURI;
    }
	////////////////////////////////////////////////////////////////////////////////////
    // End Workaround
    // Greg Boone. October 1, 2009.
	////////////////////////////////////////////////////////////////////////////////////

    GmlBaseType baseType = getGmlBaseType(validName, lUri);

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
                FdoPtr<FdoXmlLpClassDefinition> classDef = getClassDef(validName, lUri);
                if (classDef != NULL)
                    m_lpClassStack.push_back(classDef.p);

			    m_parsingStateStack.push_back(ParsingState_Feature);

                FdoPtr<FdoClassDefinition> fdoClassDef;

                if ( classDef )
                    fdoClassDef = classDef->GetClassDefinition();

			    //TODO: get class definition when XML Schema mappings not specified
			    nextFeatureHandler = curFeatureHandler->FeatureStart(m_featureContext, fdoClassDef);
			    break;
            }

		//direct gml geometry association
		case GmlBaseType_GmlDirectGeometry:
			m_parsingStateStack.push_back(ParsingState_GmlDirectGeometry);
			m_geometryHandler = FdoXmlGeometryHandler::Create(m_flags);
            m_geometryHandler->SetExpectedGmlGeometry((FdoXmlGeometryHandler::GmlGeometryType)m_activeGmlGeometryType);
            nextSaxHandler = m_geometryHandler->SkipFirstParseStep();
            if (nextSaxHandler == NULL)
                nextSaxHandler = m_geometryHandler;
			break;

		//gml geometry association
		case GmlBaseType_GmlGeometryAssociation:
			m_parsingStateStack.push_back(ParsingState_GmlGeometryAssociation);
			m_geometryHandler = FdoXmlGeometryHandler::Create(m_flags);
            m_geometryHandler->SetExpectedGmlGeometry((FdoXmlGeometryHandler::GmlGeometryType)m_activeGmlGeometryType);
			nextSaxHandler = m_geometryHandler;
			break;
		//geometry association
		case GmlBaseType_GeometryAssociation:
			m_parsingStateStack.push_back(ParsingState_GeometryAssociation);
			m_geometryHandler = FdoXmlGeometryHandler::Create(m_flags);
            m_geometryHandler->SetExpectedGmlGeometry((FdoXmlGeometryHandler::GmlGeometryType)m_activeGmlGeometryType);
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
                FdoPtr<FdoXmlLpClassDefinition> classDef = getClassDef(validName, lUri);
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
        m_featureURI = "";
		isPauseParsing = curFeatureHandler->FeatureCollectionEnd(m_featureContext);
		break;

	//feature association
	case ParsingState_FeatureAssociation:
        m_featureURI = "";
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
            if (tempByteArray != NULL)
		        isPauseParsing = curFeatureHandler->FeatureGeometricProperty(m_featureContext, 
			                L"Bounds", tempByteArray->GetData(), tempByteArray->GetCount());
	    }		
		FDO_SAFE_RELEASE(tempByteArray);
		FDO_SAFE_RELEASE(tempGeometry);
		break;

	//geometry association
	case ParsingState_GmlDirectGeometry:
	case ParsingState_GmlGeometryAssociation:
	case ParsingState_GeometryAssociation:
        {
            if (curState == ParsingState_GmlDirectGeometry) // run last step only in this case
                m_geometryHandler->RunLastParseStep(name, (FdoXmlGeometryHandler::GmlGeometryType)m_activeGmlGeometryType);
            tempGeometry =  m_geometryHandler->GetGeometry();
            FdoStringP pPropName = name;
            bool found = false;
            FdoXmlLpClassDefinition* classDef = m_lpClassStack.back();
            if (NULL != classDef)
            {
                FdoString* pBaseName = classDef->PropertyMappingNameFromGmlAlias(name);
                if (pBaseName != NULL)
                {
                    pPropName = pBaseName;
                    found = true;
                }
            }
            // only in gml geometry association add in front gml/ 
            if (!found && curState == ParsingState_GmlGeometryAssociation)
            {
                pPropName = L"gml/";
                pPropName += name;
            }
            // match the names of geometry field
            if (NULL != classDef)
            {
                FdoString* pMainGeomPropName = classDef->GetMainGeometryPropertyName();
                // in case we have only one geometry and the names are different 
                if (pMainGeomPropName != NULL && pPropName != pMainGeomPropName)
                    pPropName = pMainGeomPropName;
            }
            FdoByte* arrayData = NULL;
            FdoInt32 szArrayData = 0;
	        if (tempGeometry){
		        tempByteArray = tempGeometry->GetFgf();
                if (tempByteArray != NULL){
                arrayData = tempByteArray->GetData();
                szArrayData = tempByteArray->GetCount();
                }
            }
	        isPauseParsing = curFeatureHandler->FeatureGeometricProperty(m_featureContext, 
		                pPropName, arrayData, szArrayData);
        }
		FDO_SAFE_RELEASE(tempByteArray);
	    FDO_SAFE_RELEASE(tempGeometry);
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
            XMLSize_t len;
            XMLByte* decoded = XERCES_CPP_NAMESPACE::Base64::decode((const XMLByte*)(const char*)m_dataProperty, &len);
            if (decoded != NULL) {
                curFeatureHandler->FeatureBinaryData(m_featureContext, decoded, len);
	            delete decoded;
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
FdoXmlFeaturePropertyReaderImpl::GmlBaseType FdoXmlFeaturePropertyReaderImpl::getGmlBaseType(FdoStringP& elementName, FdoString* elementUri)
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
                // class name cannot contain "."
                if (elementName.Contains(L"."))
                    elementName = elementName.Replace(L".", L"-dot-");
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
                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Unknown;
                    if (wcscmp(elementName, L"boundedBy") == 0)
                    {
                        rv = GmlBaseType_BoundingShape;
                    }
                    else if (wcscmp(elementName, L"pointProperty") == 0	)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Point;
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"polygonProperty") == 0 ||
						wcscmp(elementName, L"surfaceProperty") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Polygon;
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"lineStringProperty") == 0 ||
						wcscmp(elementName,L"curveProperty") ==0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_LineString;
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"multiPointProperty") == 0 ||
						wcscmp(elementName,L"pointArrayProperty") ==0 )
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiPoint;
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"multiLineStringProperty") == 0 ||
						wcscmp(elementName,L"curveArrayProperty") ==0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiLineString;
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"multiPolygonProperty") == 0 ||
						wcscmp(elementName,L"surfaceArrayProperty") ==0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiPolygon;
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"multiGeometryProperty") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiGeometry;
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"geometryProperty") == 0 ||
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
                    {
                        rv = GmlBaseType_GmlGeometryAssociation;
                    }
                    else if (wcscmp(elementName, L"Point") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Point;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"Polygon") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Polygon;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"LineString") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_LineString;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"LinearRing") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_LinearRing;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"MultiPoint") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiPoint;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"MultiLineString") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiLineString;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"MultiPolygon") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiPolygon;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"MultiGeometry") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiGeometry;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                    else if (wcscmp(elementName, L"pointMember") == 0 ||
                                wcscmp(elementName, L"lineStringMember") == 0 ||
                                wcscmp(elementName, L"polygonMember") == 0 ||
                                wcscmp(elementName, L"geometryMember") == 0)
                    {
                        m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_GeometryAssociation;
                        rv = GmlBaseType_GmlDirectGeometry;
                    }
                }
                if (rv != GmlBaseType_Unknown)
                    break;
                if (m_lpClassStack.back() != NULL) { // it is a property

                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Unknown;
                    FdoXmlLpClassDefinition* classDef = m_lpClassStack.back();
                    FdoPtr<FdoXmlLpPropertyDefinition> prop = classDef->PropertyFromGml(elementUri, elementName);
                    if (prop != NULL) {
                        FdoPtr<FdoXmlElementMapping> element = prop->GetElementMapping();
                        if (element != NULL) {
                            FdoPtr<FdoXmlClassMapping> elementClass = element->GetClassMapping();
                            if (elementClass != NULL) { // it is an object or feature association or geometry association
                                FdoString* wkBaseName = elementClass->GetWkBaseName();
                                if (wcscmp(wkBaseName, FdoGml212::mPointProperty) == 0)
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Point;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mPolygonProperty) == 0 ||
									wcscmp(wkBaseName,FdoGml311::mSurfaceProperty) == 0)
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_Polygon;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mLineStringProperty) == 0 ||
									wcscmp(wkBaseName,FdoGml311::mCurveProperty) == 0)
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_LineString;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mMultiPointProperty) == 0 ||
									wcscmp(wkBaseName,FdoGml311::mPointArrayProperty) ==0 )
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiPoint;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mMultiPolygonProperty) == 0 ||
									wcscmp(wkBaseName,FdoGml311::mSurfaceArrayProperty) == 0)
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiPolygon;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mMultiLineStringProperty) == 0 ||
									wcscmp(wkBaseName,FdoGml311::mCurveArrayProperty) == 0)
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiLineString;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mMultiGeometryProperty) == 0 ||
									wcscmp(wkBaseName,FdoGml311::mGeometryArrayProperty) == 0)
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_MultiGeometry;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mGeometryAssociation) == 0)
                                {
                                    m_activeGmlGeometryType = FdoXmlGeometryHandler::GmlGeometryType_GeometryAssociation;
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mGeometryProperty) == 0 ||
                                    wcscmp(wkBaseName, FdoGml212::mAbstractGeometry) == 0) // geometry association
                                {
                                    rv = GmlBaseType_GeometryAssociation;
                                }
                                else if (wcscmp(wkBaseName, FdoGml212::mFeatureAssociation) == 0) // feature association
                                    rv = GmlBaseType_FeatureAssociation;
                                else // object property
                                {
                                    rv = GmlBaseType_GenericComplexType;
                                    // class name cannot contain "."
                                    if (elementName.Contains(L"."))
                                        elementName = elementName.Replace(L".", L"-dot-");
                                }
                            } else { // it is data property
                                // find out whether it is a LOB property
                                FdoString* className = element->GetClassName();
                                if (wcscmp(className, L"hexBinary") == 0)
                                    rv = GmlBaseType_hexBinary;
                                else if (wcscmp(className, L"base64Binary") == 0)
                                    rv = GmlBaseType_base64Binary;
                                else if (wcscmp(className, L"AbstractGeometry") == 0)
                                    rv = GmlBaseType_GeometryAssociation;
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
