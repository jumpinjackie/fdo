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
#include <sys/stat.h>
#include <sys/types.h>
#include <Fdo/Expression/Identifier.h>
#include "XmlContext.h"
#include <Fdo/Xml/Flags.h>
#include "SchemaFromExternal.h"

// Constructs an instance of a FeatureSchemaCollection using the specified argument.
FDO_API FdoFeatureSchemaCollection* FdoFeatureSchemaCollection::Create(FdoSchemaElement* parent)
{
    return new FdoFeatureSchemaCollection(parent);
}

FdoFeatureSchemaCollection::~FdoFeatureSchemaCollection()
{
    FDO_SAFE_RELEASE(m_XmlContext);
}

FdoXmlSaxContext* FdoFeatureSchemaCollection::GetSaxContext()
{
    // Create the context. The context passes this feature schema collection
    // to the sax callbacks on other feature schema elements.
    if ( !m_XmlContext ) 
        m_XmlContext = FdoSchemaXmlContext::Create( this );

    FdoXmlSaxContext* context = m_XmlContext;
    FDO_SAFE_ADDREF(context);

    return(context);
}

void FdoFeatureSchemaCollection::XmlEndDocument(FdoXmlSaxContext* context)
{
    m_XmlContext->CommitSchemas();
    m_XmlSchemaMappings = m_XmlContext->GetXmlSchemaMappings();
    FDO_SAFE_RELEASE (m_XmlContext);
}

FdoXmlSaxHandler* FdoFeatureSchemaCollection::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoFeatureSchemaP schema;
    FdoSchemaXmlContext* schemaContext = (FdoSchemaXmlContext*) context;

    // For a Schema element, create a Feature Schema and delegate the SAX
    // callback handling to it.
    if ( wcscmp( name, L"Schema" ) == 0 ) {
        FdoStringP schemaName = schemaContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"name"))->GetValue() );
        schema = schemaContext->AddSchema( schemaName, atts );

        FdoStringP targetNamespace;
        FdoXmlAttributeP att = atts->FindItem( L"targetNamespace" );

        if ( att )
            targetNamespace = att->GetValue();
        
        schemaContext->AddSchemaMapping( schemaName, targetNamespace );
    }

    // Ignore any other elements.

    return(schema);
}

void FdoFeatureSchemaCollection::_writeXml( FdoXmlWriter* writer, const FdoXmlFlags* flags )
{
    int i;

    // If writing zero or more than one Feature Schema, they need to be enclosed in
    // a single root element. When writing just one, the Feature Schema can be 
    // the root element.
    if ( GetCount() != 1 )
        writer->SetDefaultRoot(true);

    FdoSchemaXmlContextP context = FdoSchemaXmlContext::Create( (FdoXmlFlags*) flags, writer );

    // Write each Feature Schema to the XML document.
    for ( i = 0; i < GetCount(); i++ ) {
        FdoFeatureSchemaP schema = GetItem(i);
        schema->_writeXml(context);
    }

    // Report all schema errors encountered, which make it impossible to produce
    // a proper XML document.
    context->ThrowErrors();
}

FdoXmlReader* FdoFeatureSchemaCollection::GetFromInternalStylesheet() 
{
    return FdoFeatureSchema::_getFromInternalStylesheet();
}

FdoXmlReader* FdoFeatureSchemaCollection::GetFromExternalStylesheet() 
{
    FdoXmlReader* reader = NULL; 
    FdoIoMemoryStreamP stream;
#ifdef XML_DEBUG
#ifdef _WIN32
    struct _stat status;
#else
    struct stat status;
#endif
#endif

// read from file not yet working on Linux. Only needed from debugging so
// not a big deal.
#ifdef XML_DEBUG
#ifdef _WIN32
      if ( _stat("../Unmanaged/Src/Fdo/Schema/SchemaFromExternal.xslt", &status) == 0 ) {
        reader = FdoXmlReader::Create( L"../Unmanaged/Src/Fdo/Schema/SchemaFromExternal.xslt" );
    }
    else {
#endif
#endif
        stream = FdoIoMemoryStream::Create();

        for ( int i = 0; schemaFromExternal[i] != NULL; i++ ) {
            stream->Write( (FdoByte*) schemaFromExternal[i], strlen(schemaFromExternal[i]) );
        }

        stream->Reset();
        reader = FdoXmlReader::Create( stream );
#ifdef XML_DEBUG
#ifdef _WIN32
    }
#endif
#endif

    return( reader );
}

FdoIDisposableCollection* FdoFeatureSchemaCollection::FindClass(const wchar_t* className)
{
    FdoIDisposableCollection  *retClasses = FdoIDisposableCollection::Create();
    FdoPtr<FdoIdentifier> classId = FdoIdentifier::Create( className );
    FdoString*  schemaName = classId->GetSchemaName();
    if( schemaName && schemaName[0] != '\0' )
    {
        FdoPtr<FdoFeatureSchema>schema =  FindItem( schemaName );
        if( schema != NULL )
        {
            FdoPtr<FdoClassCollection> classes =  schema->GetClasses();
            FdoPtr<FdoClassDefinition>pclass = classes->FindItem( classId->GetName() );
            if( pclass )
                retClasses->Add( pclass );
        }
    }
    else // Else search through all the schemas
    {
        for ( int i = 0; i < GetCount(); i++ ) 
        {
            FdoPtr<FdoFeatureSchema>schema = GetItem(i);
            FdoPtr<FdoClassCollection> classes =  schema->GetClasses();
            FdoPtr<FdoClassDefinition>pclass = classes->FindItem( classId->GetName() );
            if( pclass )
                retClasses->Add( pclass );
        }
    }

    return retClasses;
}

FdoPhysicalSchemaMappingCollection* FdoFeatureSchemaCollection::GetXmlSchemaMappings()
{
    return FDO_SAFE_ADDREF( (FdoPhysicalSchemaMappingCollection*) m_XmlSchemaMappings );
}

/* TODO: This function needs a unit test before it can be made part of the FDO API.
FDO_API void Merge( FdoFeatureSchema* schema, bool ignoreStates = false );
void FdoFeatureSchemaCollection::Merge( FdoFeatureSchema* schema, bool ignoreStates )
{
    FdoSchemaMergeContextP context = FdoSchemaMergeContext::Create( this );

    context->SetIgnoreStates( ignoreStates );
    context->SetUpdSchema( FDO_SAFE_ADDREF(schema) );
    context->CommitSchemas();
}
*/