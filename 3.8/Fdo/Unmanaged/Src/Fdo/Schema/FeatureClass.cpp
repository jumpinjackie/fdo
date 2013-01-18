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
#include <Fdo/Schema/FeatureClass.h>
#include "XmlContext.h"


FdoFeatureClass* FdoFeatureClass::Create()
{
    return new FdoFeatureClass();
}

FdoFeatureClass* FdoFeatureClass::Create(FdoString* name, FdoString* description)
{
    return new FdoFeatureClass(name, description);
}

FdoFeatureClass::~FdoFeatureClass()
{
    FDO_SAFE_RELEASE(m_geometryCHANGED);
    FDO_SAFE_RELEASE(m_geometry);
}

void FdoFeatureClass::Dispose()
{
    delete this;
}

FdoFeatureClass::FdoFeatureClass() : FdoClassDefinition()
{
    m_geometry = NULL;
    m_geometryCHANGED = NULL;
}

FdoFeatureClass::FdoFeatureClass(FdoString* name, FdoString* description) : FdoClassDefinition(name, description)
{
    m_geometry = NULL;
    m_geometryCHANGED = NULL;
}

FdoClassType FdoFeatureClass::GetClassType()
{
    return FdoClassType_FeatureClass;
}

FdoGeometricPropertyDefinition* FdoFeatureClass::GetGeometryProperty()
{
    return FDO_SAFE_ADDREF(m_geometry);
}
    
void FdoFeatureClass::SetGeometryProperty(FdoGeometricPropertyDefinition* value)
{
    _StartChanges();
    FDO_SAFE_RELEASE(m_geometry);
    m_geometry = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoFeatureClass::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoClassDefinition::_StartChanges();

        m_geometryCHANGED = FDO_SAFE_ADDREF(m_geometry);
    }
}

void FdoFeatureClass::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoClassDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        if (m_geometry != m_geometryCHANGED)
        {
            if (m_geometry)
                m_geometry->_RejectChanges();
            FDO_SAFE_RELEASE(m_geometry);
            m_geometry = FDO_SAFE_ADDREF(m_geometryCHANGED);
        }

        // reset
        FDO_SAFE_RELEASE(m_geometryCHANGED);
    }

    if (m_geometry)
        m_geometry->_RejectChanges();
}

void FdoFeatureClass::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoClassDefinition::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        if (m_geometryCHANGED)
            m_geometryCHANGED->_AcceptChanges();
        FDO_SAFE_RELEASE(m_geometryCHANGED);
    }

    if (m_geometry)
        m_geometry->_AcceptChanges();
}

void FdoFeatureClass::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoClassDefinition::_BeginChangeProcessing();

    if (m_geometry)
        m_geometry->_BeginChangeProcessing();
}

void FdoFeatureClass::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoClassDefinition::_EndChangeProcessing();
    if (m_geometry)
        m_geometry->_EndChangeProcessing();
}

void FdoFeatureClass::Set( FdoClassDefinition* pClass, FdoSchemaMergeContext* pContext  )
{
    FdoClassDefinition::Set( pClass, pContext );

    // Base function catches class type mismatch so silently quit on type mismatch
    if ( GetClassType() == pClass->GetClassType() ) {
        if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pClass->GetElementState() == FdoSchemaElementState_Modified) ) {
            FdoGeometricPropertyP newGeomProp = ((FdoFeatureClass*)pClass)->GetGeometryProperty();
            FdoStringP oldGeomName = m_geometry ? m_geometry->GetName() : L"";
            FdoStringP newGeomName = newGeomProp ? newGeomProp->GetName() : L"";
            if ( oldGeomName != newGeomName ) {
                // GeometryProperty has changed.

                if ( (GetElementState() == FdoSchemaElementState_Added) || pContext->CanModFeatGeometry((FdoFeatureClass*)pClass) ) {
                    // Modification allowed, add a reference to be resolved later.
                    pContext->AddGeomPropRef( 
                        this, 
                        newGeomProp ? newGeomProp->GetName() : L""
                    );
                }
                else {
                    // Modification not allowed.
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_78_MODFEATGEOM),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
                }
            }
        }
    }
}

void FdoFeatureClass::CheckReferences( FdoSchemaMergeContext* pContext )
{
    // No need to check references if this element is going away.
    if ( GetElementState() != FdoSchemaElementState_Deleted ) {
        FdoInt32 idx = -1;
        FdoPropertiesP props = GetProperties();

        FdoClassDefinition::CheckReferences(pContext);

        FdoGeometricPropertyP geomProp = GetGeometryProperty();

        if ( geomProp && (geomProp->GetElementState() == FdoSchemaElementState_Deleted) ) {
            // Geometry Property is marked for deletion.
            pContext->AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_142_DELFEATGEOM),
                            (FdoString*)geomProp->GetQualifiedName(),
                            (FdoString*)GetQualifiedName()
                        )
                    )
                )
            );
        }
    }
}

void FdoFeatureClass::InitFromXml(FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Make sure FDO and XML class type agree, class type change not allowed.
    if ( (wcscmp( classTypeName, L"ClassDefinition" ) != 0) && (wcscmp( classTypeName, L"FeatureClass" ) != 0) ) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_24_CLASSTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    // Clear current geometryProperty
    FDO_SAFE_RELEASE(m_geometry);

    FdoXmlAttributeP attr = attrs->FindItem( L"geometryProperty" );

    // If new class has geometry, add a reference to be resolved later.
    if ( attr != NULL ) {
        pContext->GetMergeContext()->AddGeomPropRef( this, pContext->DecodeName(attr->GetValue()) );
    }
    else {
        // When class does not explicitly reference a geometry property, add a reference
        // to default the geometryProperty. The geometryProperty is defaulted when the 
        // Feature Class and all its ancestors contains only one geometric property.
        // This helps set the geometryProperty when the schema is from an external 
        // source.
        // However, do not default the geometryProperty if explicitly told that this
        // class does not have one.
        FdoXmlAttributeP attr = attrs->FindItem( L"hasGeometry" );
        if ( !attr || (FdoStringP(attr->GetValue()) != L"false") ) 
            pContext->GetMergeContext()->AddGeomPropRef( this, L"" );
    }

    // Initialize the Class Definition parts.
    FdoClassDefinition::InitFromXml(pContext, attrs );

}

void FdoFeatureClass::_writeXml( FdoSchemaXmlContext* pContext  )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write element for feature class.
    writer->WriteStartElement( L"FeatureClass" );

    _writeBaseXml(pContext);

    // close the element.
    writer->WriteEndElement();
}

void FdoFeatureClass::_writeBaseXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write geometryProperty as attribute
    if ( m_geometry ) 
        writer->WriteAttribute( L"geometryProperty", pContext->EncodeName(m_geometry->GetName()) );
    else
        writer->WriteAttribute( L"hasGeometry", L"false" );

    // Write ClassDefinition parts
    FdoClassDefinition::_writeXml(pContext);

}


