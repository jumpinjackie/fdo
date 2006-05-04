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
#include <Fdo/Schema/GeometricPropertyDefinition.h>
#include <Fdo/Schema/GeometricType.h>
#include "XmlContext.h"


FdoGeometricPropertyDefinition* FdoGeometricPropertyDefinition::Create()
{
    return new FdoGeometricPropertyDefinition();
}

FdoGeometricPropertyDefinition* FdoGeometricPropertyDefinition::Create(FdoString* name, FdoString* description, bool system)
{
    return new FdoGeometricPropertyDefinition(name, description, system);
}

FdoGeometricPropertyDefinition::~FdoGeometricPropertyDefinition()
{
}

void FdoGeometricPropertyDefinition::Dispose()
{
    delete this;
}

FdoGeometricPropertyDefinition::FdoGeometricPropertyDefinition() : FdoPropertyDefinition()
{
    m_geometricTypes = FdoGeometricType_All;
    m_readOnly = false;
    m_hasElevation = false;
    m_hasMeasure = false;
	m_associatedSCName =L"";

    m_geometricTypesCHANGED = FdoGeometricType_All;
    m_readOnlyCHANGED = false;
    m_hasElevationCHANGED = false;
    m_hasMeasureCHANGED = false;
	m_associatedSCNameCHANGED = L"";

    m_allDefaults = true;
}

FdoGeometricPropertyDefinition::FdoGeometricPropertyDefinition(FdoString* name, FdoString* description, bool system) :
    FdoPropertyDefinition(name,description, system)
{
    m_geometricTypes = FdoGeometricType_All;
    m_readOnly = false;
    m_hasElevation = false;
    m_hasMeasure = false;
	m_associatedSCName =L"";


    m_geometricTypesCHANGED = FdoGeometricType_All;
    m_readOnlyCHANGED = false;
    m_hasElevationCHANGED = false;
    m_hasMeasureCHANGED = false;
	m_associatedSCNameCHANGED = L"";

    m_allDefaults = true;
}

FdoPropertyType FdoGeometricPropertyDefinition::GetPropertyType()
{
    return FdoPropertyType_GeometricProperty;
}

FdoInt32 FdoGeometricPropertyDefinition::GetGeometryTypes()
{ 
    return m_geometricTypes;
}

void FdoGeometricPropertyDefinition::SetGeometryTypes(FdoInt32 value)
{
    _StartChanges();
    m_geometricTypes = value;
    if ( (m_geometricTypes != m_geometricTypesCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged) )
        SetElementState(FdoSchemaElementState_Modified);
}

bool FdoGeometricPropertyDefinition::GetReadOnly()
{
    return m_readOnly;
}

void FdoGeometricPropertyDefinition::SetReadOnly(bool value)
{
    _StartChanges();
    m_readOnly = value;
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoGeometricPropertyDefinition::GetHasElevation()
{
    return m_hasElevation;
}

void FdoGeometricPropertyDefinition::SetHasElevation(bool value)
{
    _StartChanges();
    m_hasElevation = value;
    if ( (m_hasElevation != m_hasElevationCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged) )
        SetElementState( FdoSchemaElementState_Modified );
}

bool FdoGeometricPropertyDefinition::GetHasMeasure()
{
    return m_hasMeasure;
}

void FdoGeometricPropertyDefinition::SetHasMeasure(bool value)
{
    _StartChanges();
    m_hasMeasure = value;
    if ( (m_hasMeasure != m_hasMeasureCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged) )
        SetElementState( FdoSchemaElementState_Modified );
}

void FdoGeometricPropertyDefinition::SetSpatialContextAssociation(FdoString *value)
{
    _StartChanges();
    m_associatedSCName = value;
    if ( (m_associatedSCName.ICompare(m_associatedSCNameCHANGED) != 0 ) || (GetElementState() != FdoSchemaElementState_Unchanged) )
        SetElementState( FdoSchemaElementState_Modified );
}

FdoString * FdoGeometricPropertyDefinition::GetSpatialContextAssociation()
{
    return ( m_associatedSCName.ICompare(L"") == 0 )? (FdoString *) NULL : (FdoString *) m_associatedSCName;
}

void FdoGeometricPropertyDefinition::_StartChanges()
{
    m_allDefaults = false;

    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoPropertyDefinition::_StartChanges();

        m_geometricTypesCHANGED = m_geometricTypes;
        m_readOnlyCHANGED = m_readOnly;
        m_hasElevationCHANGED = m_hasElevation;
        m_hasMeasureCHANGED = m_hasMeasure;
		m_associatedSCNameCHANGED = m_associatedSCName;
    }
}

void FdoGeometricPropertyDefinition::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        m_geometricTypes = m_geometricTypesCHANGED;
        m_readOnly = m_readOnlyCHANGED;
        m_hasElevation = m_hasElevationCHANGED;
        m_hasMeasure = m_hasMeasureCHANGED;
		m_associatedSCName = m_associatedSCNameCHANGED;
    }
}

void FdoGeometricPropertyDefinition::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_AcceptChanges();
}

void FdoGeometricPropertyDefinition::Set( FdoPropertyDefinition* pProperty, FdoSchemaXmlContext* pContext )
{
    FdoPropertyDefinition::Set(pProperty, pContext);

    FdoGeometricPropertyDefinition* pGeomProperty = (FdoGeometricPropertyDefinition*) pProperty;

    SetGeometryTypes( pGeomProperty->GetGeometryTypes() );
    SetHasElevation( pGeomProperty->GetHasElevation() );
    SetHasMeasure( pGeomProperty->GetHasMeasure() );
	SetSpatialContextAssociation( pGeomProperty->GetSpatialContextAssociation());
    SetReadOnly( pGeomProperty->GetReadOnly() );
}

void FdoGeometricPropertyDefinition::InitFromXml(const FdoString* propertyTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Error if FDO and XML property types are not the same.
    if ( wcscmp( propertyTypeName, L"GeometricProperty" ) != 0 ) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_25_PROPERTYTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    // Initialize generic property parts
    FdoPropertyDefinition::InitFromXml(pContext, attrs );

    // Reset attributes to defaults
    m_geometricTypes = FdoGeometricType_All;
    m_readOnly = false;
    m_hasElevation = false;
    m_hasMeasure = false;
	m_associatedSCName = L"";
    m_allDefaults = true;

    // Set attributes from the XML attributes.
    FdoXmlAttributeP attr = attrs->FindItem( L"readOnly" );
    if ( attr != NULL )
        SetReadOnly( FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"hasElevation" );
    if ( attr != NULL )
        SetHasElevation( FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"hasMeasure" );
    if ( attr != NULL )
        SetHasMeasure( FdoStringP(attr->GetValue()).ToBoolean());

	attr = attrs->FindItem( L"associatedSCName" );
    if ( attr != NULL )
		SetSpatialContextAssociation( attr->GetValue() );	
}

FdoXmlSaxHandler* FdoGeometricPropertyDefinition::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // Handle generic sub-elements
    FdoXmlSaxHandler* pRet = FdoSchemaElement::XmlStartElement(context, uri, name, qname, atts);
    FdoPtr<FdoPropertyDefinition> pProp;

    if ( !pRet ) {
        // Not generic, so handle here. Only Geometric types are handled.
        if ( wcscmp(name, L"GeometricTypes") == 0 ) {
            // starting to read geometric types. Initialize the read
            m_XmlGeometricTypes = 0;
        }

        if ( wcscmp(name, L"GeometricType") == 0 ) {

            // Set up a SAX handler to read the geometric type from the
            // element content.
            m_XmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = m_XmlContentHandler;
        }
    }

    return(pRet);
}

FdoBoolean FdoGeometricPropertyDefinition::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname 
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // End any generic sub-elements.
    FdoSchemaElement::XmlEndElement(context, uri, name, qname);

    if ( wcscmp(name, L"GeometricTypes") == 0 ) {
        // all geometric types have been read so set the member.
        // set to all types if none were read.
        SetGeometryTypes(m_XmlGeometricTypes == 0 ? FdoGeometricType_All : m_XmlGeometricTypes );
    }

    if ( wcscmp(name, L"GeometricType") == 0 ) {
        FdoStringP sGeomType = m_XmlContentHandler->GetString();

        // Convert the string to a bit
        if ( sGeomType == L"point" )
            m_XmlGeometricTypes |= FdoGeometricType_Point;
        else if ( sGeomType == L"curve" )
            m_XmlGeometricTypes |= FdoGeometricType_Curve;
        else if ( sGeomType == L"surface" )
            m_XmlGeometricTypes |= FdoGeometricType_Surface;
        else if ( sGeomType == L"solid" )
            m_XmlGeometricTypes |= FdoGeometricType_Solid;

    }

    return(false);
}


void FdoGeometricPropertyDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write the geometric property element
    writer->WriteStartElement( L"GeometricProperty" );

    // Write the geometric attributes.
    if ( m_readOnly ) 
        writer->WriteAttribute( L"readOnly", L"true" );

    writer->WriteAttribute( L"hasElevation", m_hasElevation ? L"true" : L"false" );
    writer->WriteAttribute( L"hasMeasure", m_hasMeasure ? L"true" : L"false" );

    // Write the geometric attributes.
    if ( m_associatedSCName.GetLength() > 0 ) 
        writer->WriteAttribute( L"associatedSCName", m_associatedSCName );

    // Write the generic property attributes
    FdoPropertyDefinition::_writeXml(pContext);

    // Write the geometric types
    writer->WriteStartElement( L"GeometricTypes" );
    
    // Convert each masked bit into a string
    if ( (m_geometricTypes & FdoGeometricType_Point) != 0 ) {
        writer->WriteStartElement( L"GeometricType" );
        writer->WriteCharacters( L"point" );
        writer->WriteEndElement();
    }

    if ( (m_geometricTypes & FdoGeometricType_Curve) != 0 ) {
        writer->WriteStartElement( L"GeometricType" );
        writer->WriteCharacters( L"curve" );
        writer->WriteEndElement();
    }

    if ( (m_geometricTypes & FdoGeometricType_Surface) != 0 ) {
        writer->WriteStartElement( L"GeometricType" );
        writer->WriteCharacters( L"surface" );
        writer->WriteEndElement();
    }

    if ( (m_geometricTypes & FdoGeometricType_Solid) != 0 ) {
        writer->WriteStartElement( L"GeometricType" );
        writer->WriteCharacters( L"solid" );
        writer->WriteEndElement();
    }

    // close the geometry types
    writer->WriteEndElement();

    // close the geometric property.
    writer->WriteEndElement();
}

bool FdoGeometricPropertyDefinition::GetAllDefaults()
{
    return m_allDefaults;
}


