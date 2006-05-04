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
#include <Fdo/Schema/FeatureSchema.h>
#include <Fdo/Schema/Class.h>
#include <Fdo/Schema/FeatureClass.h>
#include <Fdo/Schema/NetworkNodeFeatureClass.h>
#include "XmlContext.h"
#include "SchemaFromInternal.h"


FdoFeatureSchema* FdoFeatureSchema::Create()
{
    return new FdoFeatureSchema();
}

FdoFeatureSchema* FdoFeatureSchema::Create(FdoString* name, FdoString* description)
{
    return new FdoFeatureSchema(name, description);
}

FdoFeatureSchema::~FdoFeatureSchema()
{
    FDO_SAFE_RELEASE(m_classes);
}

void FdoFeatureSchema::Dispose()
{
    delete this;
}

FdoFeatureSchema::FdoFeatureSchema() 
{
    m_classes = FdoClassCollection::Create(this);
}

FdoFeatureSchema::FdoFeatureSchema(FdoString* name, FdoString* description) : FdoSchemaElement(name, description)
{
    m_classes = FdoClassCollection::Create(this);
}

FdoClassCollection* FdoFeatureSchema::GetClasses()
{
    return FDO_SAFE_ADDREF(m_classes);
}

void FdoFeatureSchema::AcceptChanges()
{
    _BeginChangeProcessing();
    m_classes->_BeginChangeProcessing();

    _AcceptChanges();
    m_classes->_AcceptChanges();

    m_classes->_EndChangeProcessing();
    _EndChangeProcessing();
}

void FdoFeatureSchema::RejectChanges()
{
    _BeginChangeProcessing();
    m_classes->_BeginChangeProcessing();

    _RejectChanges();
    m_classes->_RejectChanges();

    m_classes->_EndChangeProcessing();
    _EndChangeProcessing();
}

void FdoFeatureSchema::Set( FdoFeatureSchema* pSchema, FdoSchemaXmlContext* pContext )
{
    FdoInt32 newIdx = -1;

    FdoSchemaElement::Set(pSchema, pContext);

    // Merge the given schema's classes into this class.

    FdoClassesP newClasses = pSchema->GetClasses();

    // For each new class
    for ( newIdx = (newClasses->GetCount() - 1); newIdx >= 0; newIdx-- ) {

        FdoClassDefinitionP newClass = newClasses->GetItem(newIdx);
        // Remove it from the new schema.
        newClasses->RemoveAt(newIdx);

        FdoClassDefinitionP oldClass = m_classes->FindItem( newClass->GetName() );

        if ( oldClass ) {
            // Class already in schema, do a merge
            oldClass->Set(newClass, pContext);
        }
        else {
            // Not in schema, just add it.
            m_classes->Add(newClass);
        }
    }
}

FdoXmlReader* FdoFeatureSchema::GetFromInternalStylesheet() 
{
    return _getFromInternalStylesheet();
}

FdoXmlReader* FdoFeatureSchema::_getFromInternalStylesheet() 
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

#ifdef XML_DEBUG
#ifdef _WIN32
    if ( _stat("../Unmanaged/Src/Fdo/Schema/SchemaFromInternal.xslt", &status) == 0 ) {
#else
    if ( lstat("../Unmanaged/Src/Fdo/Schema/SchemaFromInternal.xslt", &status) == 0 ) {
#endif
        reader = FdoXmlReader::Create( L"../Unmanaged/Src/Fdo/Schema/SchemaFromInternal.xslt" );
    }
    else {
#endif
        stream = FdoIoMemoryStream::Create();

        for ( int i = 0; schemaFromInternal[i] != NULL; i++ ) {
            stream->Write( (FdoByte*) schemaFromInternal[i], strlen(schemaFromInternal[i]) );
        }

        stream->Reset();
        reader = FdoXmlReader::Create( stream );
#ifdef XML_DEBUG
    }
#endif

    return( reader );
}



FdoXmlSaxHandler* FdoFeatureSchema::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // Handle generic parts of the sub-element.
    FdoXmlSaxHandler* pRet = FdoSchemaElement::XmlStartElement(context, uri, name, qname, atts);

    if ( !pRet ) {
        // Not a generic sub-element so must be a class; handle it here.

        // Make sure it is a class.
        if ( (wcscmp(name, L"FeatureClass") == 0) || 
             (wcscmp(name, L"Class") == 0) ||
             (wcscmp(name, L"ClassDefinition") == 0) ||
             (wcscmp(name, L"NetworkLayerClass") == 0) ||
             (wcscmp(name, L"NetworkClass") == 0) ||
             (wcscmp(name, L"NetworkNodeFeatureClass") == 0) ||
             (wcscmp(name, L"NetworkLinkFeatureClass") == 0)

        ) {
            // Name attribute must always be present
            FdoStringP className = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"name"))->GetValue() );

            // Check if class already in schema
            FdoPtr<FdoClassDefinition> pClass = m_classes->FindItem(className);

            if ( pClass == NULL ) {
                // Class not in schema so create new class of the right
                // class type. Set description blank for now, the 
                // real description will be subsequently deserialized.
                if ( wcscmp(name, L"FeatureClass") == 0 )
                    pClass = FdoFeatureClass::Create( className, L"" );
                else if ( wcscmp(name, L"Class") == 0 )
                    pClass = FdoClass::Create( className, L"" );
                else if ( wcscmp(name, L"NetworkLayerClass") == 0 )
                    pClass = FdoNetworkLayerClass::Create( className, L"" );
                else if ( wcscmp(name, L"NetworkClass") == 0 )
                    pClass = FdoNetworkClass::Create( className, L"" );
                else if ( wcscmp(name, L"NetworkNodeFeatureClass") == 0 )
                    pClass = FdoNetworkNodeFeatureClass::Create( className, L"" );
                else if ( wcscmp(name, L"NetworkLinkFeatureClass") == 0 )
                    pClass = FdoNetworkLinkFeatureClass::Create( className, L"" );
                else
                    // Class type not yet known, the following finds the 
                    // type of its base class and creates this class of the 
                    // same type.
                    pClass = fdoContext->CreateClass( GetName(), className, atts );

                if ( pClass != NULL ) 
                    m_classes->Add(pClass);
            }

            if ( pClass != NULL ) {
                // Initialze the class from its XML attributes
                pClass->InitFromXml( name, fdoContext, atts );

                // Make the class the new SAX Handler, to handle
                // its properties and other sub-elements.
                pRet = pClass;
            }
        }

        if ( wcscmp(name, L"ElementMapping") == 0 ) {
            // Handle schema mapping for GML element
            FdoStringP elementName = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"name"))->GetValue() );
            FdoStringP classSchema = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"classSchema"))->GetValue() );
            FdoStringP className = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"className"))->GetValue() );
        
            fdoContext->AddElementMapping( GetName(), elementName, classSchema, className );
        }

        if ( wcscmp(name, L"ClassMapping") == 0 ) {
            // Handle schema mapping for class-complexType
            FdoStringP className = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"name"))->GetValue() );

            FdoStringP gmlName;
            FdoStringP wkSchema;
            FdoStringP wkClass;

            FdoXmlAttributeP att = atts->FindItem(L"gmlName");
            if ( att )
                gmlName = att->GetValue();

            att = atts->FindItem(L"wkSchema");
            if ( att )
                wkSchema = fdoContext->DecodeName( att->GetValue() );

            att = atts->FindItem(L"wkClass");
            if ( att )
               wkClass = fdoContext->DecodeName( att->GetValue() );
        
            fdoContext->AddClassMapping( GetName(), className, gmlName, wkSchema, wkClass );
        }
    }

    return(pRet);
}


void FdoFeatureSchema::_writeXml( FdoXmlWriter* writer, const FdoXmlFlags* flags  )
{
    FdoSchemaXmlContextP context = FdoSchemaXmlContext::Create( (FdoXmlFlags*) flags, writer );

    // Write the Schema
    _writeXml(context);

    // Report all schema errors encountered, which make it impossible to produce
    // a proper XML document.
    context->ThrowErrors();
}

void FdoFeatureSchema::_writeXml( FdoSchemaXmlContext* pContext )
{
    int                 i;
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write the Feature Schema element.
    writer->WriteStartElement( L"Schema" );

    // Write the generic attributes and sub-elements
    FdoSchemaElement::_writeXml(pContext);

    // Write each class
    for ( i = 0; i < m_classes->GetCount(); i++ ) 
        FdoClassDefinitionP(m_classes->GetItem(i))->_writeXml(pContext);

    // Close the Feature Schema element.
    writer->WriteEndElement();
}

