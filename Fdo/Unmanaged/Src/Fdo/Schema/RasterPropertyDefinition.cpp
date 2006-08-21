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
#include <Fdo/Schema/RasterPropertyDefinition.h>

#include <Fdo/Schema/DataType.h>
#include <Fdo/Schema/SchemaException.h>
#include "XmlContext.h"

// Constructs a default instance of an FdoRasterPropertyDefinition.
FdoRasterPropertyDefinition::FdoRasterPropertyDefinition () :
    FdoPropertyDefinition (),
    m_readOnly (false),
    m_nullable (true),
    m_model (NULL),
    m_sizeX (1024),
    m_sizeY (1024),
    m_readOnlyCHANGED (false),
    m_nullableCHANGED (true),
    m_modelCHANGED (NULL),
    m_sizeXCHANGED (-1),
    m_sizeYCHANGED (-1)
{
}

// Constructs an instance of an FdoRasterPropertyDefinition using the
// specified arguments.
FdoRasterPropertyDefinition::FdoRasterPropertyDefinition (
    FdoString* name, FdoString* description, bool system) :
    FdoPropertyDefinition (name, description, system),
    m_readOnly (false),
    m_nullable (true),
    m_model (NULL),
    m_sizeX (1024),
    m_sizeY (1024),
    m_readOnlyCHANGED (false),
    m_nullableCHANGED (true),
    m_modelCHANGED (NULL),
    m_sizeXCHANGED (-1),
    m_sizeYCHANGED (-1)
{
}

// Destructor.
FdoRasterPropertyDefinition::~FdoRasterPropertyDefinition ()
{
    FDO_SAFE_RELEASE(m_model);
}

//
// FdoIDisposable interface.
//

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void FdoRasterPropertyDefinition::Dispose ()
{
    delete this;
}

//
// Static instance creation.
//

/// <summary>Constructs a default instance of an FdoRasterPropertyDefinition.</summary>
/// <returns>Returns FdoRasterPropertyDefinition</returns> 
FdoRasterPropertyDefinition* FdoRasterPropertyDefinition::Create ()
{
    return new FdoRasterPropertyDefinition ();
}

/// <summary>Constructs an instance of an FdoRasterPropertyDefinition using the
/// specified arguments.</summary>
/// <param name="name">Input name</param> 
/// <param name="description">Input description</param> 
/// <returns>Returns FdoRasterPropertyDefinition</returns> 
FdoRasterPropertyDefinition* FdoRasterPropertyDefinition::Create (
    FdoString* name, FdoString* description, bool system)
{
    return new FdoRasterPropertyDefinition (name, description, system);
}

//
// FdoPropertyDefinition interface.
//

FdoPropertyType FdoRasterPropertyDefinition::GetPropertyType ()
{
	return (FdoPropertyType_RasterProperty);
}

//
// FdoSchemaElement interface.
//

void FdoRasterPropertyDefinition::_StartChanges ()
{
    if (0 != (m_changeInfoState & (CHANGEINFO_PRESENT | CHANGEINFO_PROCESSING)))
    {
        superclass::_StartChanges ();

        m_readOnlyCHANGED = m_readOnly;
        m_nullableCHANGED = m_nullable;
        m_modelCHANGED = m_model;
        m_sizeXCHANGED = m_sizeX;
        m_sizeYCHANGED = m_sizeY;
        m_scAssociationCHANGED = m_spatialContextAssociation;
    }
}

void FdoRasterPropertyDefinition::_RejectChanges()
{
    if (0 == (m_changeInfoState & CHANGEINFO_PROCESSED))
    {   // not already processed
        superclass::_RejectChanges();
        // restore
        if (0 != (m_changeInfoState & CHANGEINFO_PRESENT))
        {
            m_readOnly = m_readOnlyCHANGED;
            m_readOnlyCHANGED = false;
            m_nullable = m_nullableCHANGED;
            m_nullableCHANGED = true;
            m_model = m_modelCHANGED;
            m_modelCHANGED = NULL;
            m_sizeX = m_sizeXCHANGED;
            m_sizeXCHANGED = -1;
            m_sizeY = m_sizeYCHANGED;
            m_sizeYCHANGED = -1;
		    m_spatialContextAssociation = m_scAssociationCHANGED;
            m_scAssociationCHANGED = L"";
        }
    }
}

void FdoRasterPropertyDefinition::_AcceptChanges()
{
    if (0 == (m_changeInfoState & CHANGEINFO_PROCESSED))
    {   // not already processed
        superclass::_AcceptChanges ();
        // reset
        m_readOnlyCHANGED = false;
        m_nullableCHANGED = true;
        if ((m_model != m_modelCHANGED) && (NULL != m_modelCHANGED))
            m_modelCHANGED->Release ();
        m_modelCHANGED = NULL;
        m_sizeXCHANGED = -1;
        m_sizeYCHANGED = -1;
        m_scAssociationCHANGED = L"";
    }
}

//
// Attributes.
//

/// <summary>Returns a Boolean value that indicates if this property is read-only.</summary>
/// <returns>Returns a Boolean value</returns> 
bool FdoRasterPropertyDefinition::GetReadOnly ()
{
    return (m_readOnly);
}

/// <summary>Sets a Boolean value that indicates if this property is read-only.</summary>
/// <param name="value">Input a Boolean value that indicates if this property is read-only</param> 
/// <returns>Returns nothing</returns> 
void FdoRasterPropertyDefinition::SetReadOnly (bool value)
{
    _StartChanges ();
    m_readOnly = value;
    SetElementState (FdoSchemaElementState_Modified);
}

/// <summary>Returns a Boolean value that indicates if this property's value can be
/// null.</summary>
/// <returns>Returns a Boolean value</returns> 
bool FdoRasterPropertyDefinition::GetNullable ()
{
    return m_nullable;
}

    /// <summary>Sets a Boolean value that indicates if this property's value can be
    /// null.</summary>
    /// <param name="value">Input a Boolean value that indicates if this property's value can be
    /// null</param> 
    /// <returns>Returns nothing</returns> 
void FdoRasterPropertyDefinition::SetNullable (bool value)
{
    _StartChanges ();
    m_nullable = value;
    SetElementState (FdoSchemaElementState_Modified);
}

/// <summary>Gets the default data model used by this raster property.</summary>
/// <returns>Returns the current default data model.</returns> 
FDO_API FdoRasterDataModel* FdoRasterPropertyDefinition::GetDefaultDataModel ()
{
    FDO_SAFE_ADDREF(m_model);
    return (m_model);
}

/// <summary>Sets the default data model used by this raster property.
/// Allowed values are only those data models that are acceptable to the
/// SupportsDataModel capability.</summary>
/// <param name="datamodel">The datamodel to be used for newly created
/// rasters, or the default datamodel to be used when returning raster data.</param>
FDO_API void FdoRasterPropertyDefinition::SetDefaultDataModel (FdoRasterDataModel* datamodel)
{
    _StartChanges ();
    FDO_SAFE_RELEASE(m_model);
    m_model = datamodel;
    FDO_SAFE_ADDREF(m_model);
    SetElementState (FdoSchemaElementState_Modified);
}

/// <summary>Gets the default size of image file in the horizontal
/// direction in pixels (number of columns).</summary>
/// <returns>Returns the current default horizontal image size in pixels
/// (number of columns).</returns> 
FDO_API FdoInt32 FdoRasterPropertyDefinition::GetDefaultImageXSize ()
{
    return (m_sizeX);
}

/// <summary>Sets the default size of image file in the horizontal
/// direction in pixels (number of columns).</summary>
/// <param name="size">The desired default horizontal image size in pixels
/// (number of columns).</param> 
FDO_API void FdoRasterPropertyDefinition::SetDefaultImageXSize (FdoInt32 size)
{
    _StartChanges ();
    m_sizeX = size;
    SetElementState (FdoSchemaElementState_Modified);
}

/// <summary>Gets the default size of image file in the vertical
/// direction in pixels (number of rows).</summary>
/// <returns>Returns the current default vertical image size in pixels
/// (number of rows).</returns> 
FDO_API FdoInt32 FdoRasterPropertyDefinition::GetDefaultImageYSize ()
{
    return (m_sizeY);
}

/// <summary>Sets the default size of image file in the vertical
/// direction in pixels (number of rows).</summary>
/// <param name="size">The desired default vertical image size in pixels
/// (number of rows).</param> 
FDO_API void FdoRasterPropertyDefinition::SetDefaultImageYSize (FdoInt32 size)
{
    _StartChanges ();
    m_sizeY = size;
    SetElementState (FdoSchemaElementState_Modified);
}

void FdoRasterPropertyDefinition::Set( FdoPropertyDefinition* pProperty, FdoSchemaMergeContext* pContext )
{
    FdoPropertyDefinition::Set(pProperty, pContext);

    // Base function catches property type mismatch so silently quit on type mismatch
    if ( GetPropertyType() == pProperty->GetPropertyType() ) {
        if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pProperty->GetElementState() == FdoSchemaElementState_Modified) ) {
            FdoRasterPropertyDefinition* pRasterProperty = (FdoRasterPropertyDefinition*) pProperty;

            // Set each member from the given raster property. The same pattern is followed 
            // for each:
            //
            // If new and old values differ
            //    If modification allowed (always allowed if this is a new property).
            //      Perform the modification
            //    else
            //      log an error
            //    end if
            //  end if

            // ReadOnly setting

            if ( GetReadOnly() != pRasterProperty->GetReadOnly() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModRasterReadOnly(pRasterProperty)) ) 
                    SetReadOnly( pRasterProperty->GetReadOnly() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(pRasterProperty->GetReadOnly() ? SCHEMA_95_MODPROPRDONLY : SCHEMA_96_MODPROPWRITABLE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Nullability

            if ( GetNullable() != pRasterProperty->GetNullable() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModRasterNullable(pRasterProperty)) ) 
                    SetNullable( pRasterProperty->GetNullable() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(pRasterProperty->GetNullable() ? SCHEMA_90_MODPROPNULLABLE : SCHEMA_91_MODPROPNNULLABLE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Data Model

            FdoPtr<FdoRasterDataModel> dataModel = pRasterProperty->GetDefaultDataModel();
            if ( m_model || dataModel ) {
                if ( ((m_model == NULL) != (dataModel == NULL)) || (!m_model->Equals(dataModel)) ) {
                    if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModRasterModel(pRasterProperty)) ) {
/* 
For the purposes of XML reading and SDF Provider ApplySchema, reusing the datamodel
is fine, but is not ok in the general case. 
TODO copy the datamodel
                        FdoPtr<FdoRasterDataModel> dataModelCopy = dataModel ? dataModel->CreateCopy() : (FdoDataModel*) NULL;
                        SetDefaultDataModel( dataModelCopy );
*/
                        SetDefaultDataModel( dataModel );
                    }
                    else {
                        pContext->AddError( 
                            FdoSchemaExceptionP(
                                FdoSchemaException::Create(
                                    FdoException::NLSGetMessage(
                                        FDO_NLSID(SCHEMA_115_MODRASTERMODEL),
                                        (FdoString*) GetQualifiedName()
                                    )
                                )
                            )
                        );
                    }
                }
            }

            // Image Size

            if ( GetDefaultImageXSize() != pRasterProperty->GetDefaultImageXSize() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModRasterXSize(pRasterProperty)) ) 
                    SetDefaultImageXSize( pRasterProperty->GetDefaultImageXSize() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_116_MODRASTERX),
                                    (FdoString*) GetQualifiedName(),
                                    GetDefaultImageXSize(),
                                    pRasterProperty->GetDefaultImageXSize()
                                )
                            )
                        )
                    );
            }

            if ( GetDefaultImageYSize() != pRasterProperty->GetDefaultImageYSize() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModRasterYSize(pRasterProperty)) ) 
                    SetDefaultImageYSize( pRasterProperty->GetDefaultImageYSize() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_117_MODRASTERY),
                                    (FdoString*) GetQualifiedName(),
                                    GetDefaultImageYSize(),
                                    pRasterProperty->GetDefaultImageYSize()
                                )
                            )
                        )
                    );
            }

            // Spatial Context

            if ( FdoStringP(GetSpatialContextAssociation()) != FdoStringP(pRasterProperty->GetSpatialContextAssociation()) ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModRasterSC(pRasterProperty)) ) 
                    SetSpatialContextAssociation( pRasterProperty->GetSpatialContextAssociation() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_102_MODPROPSC),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) FdoStringP(GetSpatialContextAssociation()),
                                    (FdoString*) FdoStringP(pRasterProperty->GetSpatialContextAssociation())
                                )
                            )
                        )
                    );
            }
        }
    }
}

void FdoRasterPropertyDefinition::InitFromXml(const FdoString* propertyTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Error if FDO and XML property types are not the same.
    if ( wcscmp( propertyTypeName, L"RasterProperty" ) != 0 ) {
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

    // Initialize generic parts
    FdoPropertyDefinition::InitFromXml(pContext, attrs );

    // Reset raster property attributes to defaults.
    m_readOnly = false;
    m_nullable = false;
    FDO_SAFE_RELEASE(m_model);
    m_sizeX = 1024;
    m_sizeY = 1024;
    m_spatialContextAssociation = L"";

    FdoXmlAttributeP attr = attrs->FindItem( L"readOnly" );

    if ( attr != NULL ) 
        SetReadOnly(FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"nullable" );
    if ( attr != NULL ) 
        SetNullable(FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"sizeX" );
    if ( attr != NULL ) 
        SetDefaultImageXSize(FdoStringP(attr->GetValue()).ToLong());

    attr = attrs->FindItem( L"sizeY" );
    if ( attr != NULL ) 
        SetDefaultImageYSize(FdoStringP(attr->GetValue()).ToLong());

    attr = attrs->FindItem( L"srsName" );
    if ( attr != NULL ) 
        SetSpatialContextAssociation(attr->GetValue());

    FdoPtr<FdoRasterDataModel> dataModel = FdoRasterDataModel::Create();
    bool bSetModel = false;
    FdoXmlAttributeP attrDataModelType = attrs->FindItem( L"DataModelType" );
    if (attrDataModelType != NULL) {
        dataModel->SetDataModelType(StringToDataModelType(FdoStringP(attrDataModelType->GetValue())));
        bSetModel = true;
    }

    FdoXmlAttributeP attrBitsPerPixel = attrs->FindItem( L"BitsPerPixel" );
    if (attrBitsPerPixel != NULL) {
        dataModel->SetBitsPerPixel(FdoStringP(attrBitsPerPixel->GetValue()).ToLong());
        bSetModel = true;    
    }

    FdoXmlAttributeP attrOrganization = attrs->FindItem( L"Organization" );
    if (attrOrganization != NULL) {
        dataModel->SetOrganization(StringToDataOrganization(FdoStringP(attrOrganization->GetValue())));
        bSetModel = true;
    }

    FdoXmlAttributeP attrDataType = attrs->FindItem( L"DataType" );
    if (attrDataType != NULL) {
        dataModel->SetDataType(StringToDataType(FdoStringP(attrDataType->GetValue())));
        bSetModel = true;
    }

    FdoXmlAttributeP attrTileSizeX = attrs->FindItem( L"TileSizeX" );
    if (attrTileSizeX != NULL) {
        dataModel->SetTileSizeX(FdoStringP(attrTileSizeX->GetValue()).ToLong());
        bSetModel = true;
    }

    FdoXmlAttributeP attrTileSizeY = attrs->FindItem( L"TileSizeY" );
    if (attrTileSizeY != NULL) {
        dataModel->SetTileSizeY(FdoStringP(attrTileSizeY->GetValue()).ToLong());  
        bSetModel = true;
    }

    if (bSetModel)
        SetDefaultDataModel(dataModel);
}

void FdoRasterPropertyDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write a raster property element
    writer->WriteStartElement( L"RasterProperty" );

    if ( m_readOnly ) 
        writer->WriteAttribute( L"readOnly", L"true" );

    if ( m_nullable ) 
        writer->WriteAttribute( L"nullable", L"true" );

    if ( m_sizeX ) 
        writer->WriteAttribute( L"sizeX", FdoStringP::Format(L"%d", m_sizeX) );

    if ( m_sizeY ) 
        writer->WriteAttribute( L"sizeY", FdoStringP::Format(L"%d", m_sizeY) );

    if ( m_spatialContextAssociation.GetLength() ) 
        writer->WriteAttribute( L"srsName", (FdoString*)m_spatialContextAssociation );

    if (m_model)
    {
        writer->WriteAttribute( L"DataModelType", (FdoString*)DataModelTypeToString(m_model->GetDataModelType()) );

        if ( m_model->GetBitsPerPixel() )
            writer->WriteAttribute( L"BitsPerPixel", FdoStringP::Format(L"%d", m_model->GetBitsPerPixel()) );
        
        writer->WriteAttribute( L"Organization", (FdoString*)DataOrganizationToString(m_model->GetOrganization()) );

        writer->WriteAttribute( L"DataType", (FdoString*)DataTypeToString(m_model->GetDataType()) );

        if ( m_model->GetTileSizeX() )
            writer->WriteAttribute( L"TileSizeX", FdoStringP::Format(L"%d", m_model->GetTileSizeX()) );

        if ( m_model->GetTileSizeY() )
            writer->WriteAttribute( L"TileSizeY", FdoStringP::Format(L"%d", m_model->GetTileSizeY()) );

    }
    // Write the generic attributes and sub-elements
    FdoPropertyDefinition::_writeXml(pContext);

    // close the raster property element.
    writer->WriteEndElement();
}


FdoStringP FdoRasterPropertyDefinition::DataModelTypeToString(FdoRasterDataModelType type)
{
    FdoStringP typeString;

    switch (type)
    {
        case FdoRasterDataModelType_Data:
            typeString = L"Data";
            break;

        case FdoRasterDataModelType_Bitonal:
            typeString = L"Bitonal";
            break;

        case FdoRasterDataModelType_Gray:
            typeString = L"Gray";
            break;

        case FdoRasterDataModelType_RGB:
            typeString = L"RGB";
            break;

        case FdoRasterDataModelType_RGBA:
            typeString = L"RGBA";
            break;

        case FdoRasterDataModelType_Palette:
            typeString = L"Palette";
            break;

        case FdoRasterDataModelType_Unknown:
        default:
            typeString = L"Unknown";

    }

    return typeString;
}


FdoStringP FdoRasterPropertyDefinition::DataOrganizationToString(FdoRasterDataOrganization organization)
{
    FdoStringP organizationString;
    switch(organization)
    {
        case FdoRasterDataOrganization_Pixel:
            organizationString = L"Pixel";
            break;

        case FdoRasterDataOrganization_Row:
            organizationString = L"Row";
            break;

        case FdoRasterDataOrganization_Image:
        default:
            organizationString = L"Image";
            break;
    }

    return organizationString;

}

FdoStringP FdoRasterPropertyDefinition::DataTypeToString(FdoRasterDataType dataType)
{
    FdoStringP dataTypeString;
    switch(dataType)
    {
        case FdoRasterDataType_UnsignedInteger:
            dataTypeString = L"UnsignedInteger";
            break;
        case FdoRasterDataType_Integer:
            dataTypeString = L"Integer";
            break;
        case FdoRasterDataType_Float:
            dataTypeString = L"Float";
            break;
        case FdoRasterDataType_Double:
            dataTypeString = L"Double";
            break;
        default:
            dataTypeString = L"Unknown";
            break;
    }

    return dataTypeString;
}


FdoRasterDataModelType FdoRasterPropertyDefinition::StringToDataModelType(FdoString *typeString)
{
    FdoRasterDataModelType type;

    if (wcscmp(typeString, L"Bitonal") == 0)
        type = FdoRasterDataModelType_Bitonal;
    else if (wcscmp(typeString, L"Gray") == 0)
        type = FdoRasterDataModelType_Gray;
    else if (wcscmp(typeString, L"RGB") == 0)
        type = FdoRasterDataModelType_RGB;
    else if (wcscmp(typeString, L"RGBA") == 0)
        type = FdoRasterDataModelType_RGBA;
    else if (wcscmp(typeString, L"Palette") == 0)
        type = FdoRasterDataModelType_Palette;
    else if (wcscmp(typeString, L"Data") == 0)
        type = FdoRasterDataModelType_Data;
    else
        type = FdoRasterDataModelType_Unknown;

    return type;
}


FdoRasterDataOrganization FdoRasterPropertyDefinition::StringToDataOrganization(FdoString *organizationString)
{
    FdoRasterDataOrganization organization;

    if (wcscmp(organizationString, L"Pixel") == 0)
        organization = FdoRasterDataOrganization_Pixel;
    else if (wcscmp(organizationString, L"Row") == 0)
        organization = FdoRasterDataOrganization_Row;
    else
        organization = FdoRasterDataOrganization_Image;

    return organization;

}

FdoRasterDataType FdoRasterPropertyDefinition::StringToDataType(FdoString *typeString)
{
    FdoRasterDataType dataType;

    if (wcscmp(typeString, L"UnsignedInteger") == 0)
        dataType = FdoRasterDataType_UnsignedInteger;
    else if (wcscmp(typeString, L"Integer") == 0)
        dataType = FdoRasterDataType_Integer;
    else if (wcscmp(typeString, L"Float") == 0)
        dataType = FdoRasterDataType_Float;
    else if (wcscmp(typeString, L"Double") == 0)
        dataType = FdoRasterDataType_Double;
    else
        dataType = FdoRasterDataType_Unknown;

    return dataType;

}

void FdoRasterPropertyDefinition::SetSpatialContextAssociation(FdoString *spatialContextName)
{
    _StartChanges ();
    m_spatialContextAssociation = spatialContextName;
    SetElementState (FdoSchemaElementState_Modified);
}


FdoString * FdoRasterPropertyDefinition::GetSpatialContextAssociation()
{
    return m_spatialContextAssociation;
}

