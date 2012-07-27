/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"

#include <Rdbms/Override/RdbmsOvClassDefinition.h>
#include "Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h"


FdoRdbmsOvClassDefinition::FdoRdbmsOvClassDefinition()
{
    Init();
}

FdoRdbmsOvClassDefinition::FdoRdbmsOvClassDefinition(FdoString *name) : FdoPhysicalClassMapping(name)
{
    Init();
}

FdoRdbmsOvClassDefinition::~FdoRdbmsOvClassDefinition()
{
}

FdoRdbmsOvReadOnlyPropertyDefinitionCollection *FdoRdbmsOvClassDefinition::GetProperties()
{
    return FdoRdbmsOvReadOnlyPropertyDefinitionCollection::Create(mProperties);
}

FdoRdbmsOvTable *FdoRdbmsOvClassDefinition::GetTable()
{
    return FDO_SAFE_ADDREF( (FdoRdbmsOvTable*) mTable );
}

FdoSmOvTableMappingType FdoRdbmsOvClassDefinition::GetTableMapping()
{
    return mMappingType;
}

void FdoRdbmsOvClassDefinition::SetTableMapping(FdoSmOvTableMappingType mappingType)
{
    mMappingType = mappingType;
}

void FdoRdbmsOvClassDefinition::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoRdbmsOvClassDefinition::Init()
{
    mMappingType = FdoSmOvTableMappingType_Default;
    mCurrProp = NULL;

    mProperties = FdoRdbmsOvPropertyDefinitionCollection::Create(this);
//    mIndices = FdoRdbmsOvIndexCollection::Create(this); for future
//    mUniqueKeys = FdoRdbmsOvUniqueKeyCollection::Create(this); for future

    mElementSubElements = FdoStringCollection::Create( L"Column GeometricColumn PropertyMapping", L" " );
    mOpSubElements = FdoStringCollection::Create( L"PropertyMappingClass PropertyMappingConcrete PropertyMappingSingle", L" " );
}

void FdoRdbmsOvClassDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Get the current error level

    FdoXmlFlags::ErrorLevel errorLevel = FdoXmlFlags::ErrorLevel_VeryLow;
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    if ( fdoContext )
        errorLevel = FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel();

    FdoPhysicalClassMapping::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"tableMapping" );
    if ( att ) {
        try {
            mMappingType = FdoSmOvTableMappingTypeMapper::String2Type(att->GetValue());
        }
        catch ( FdoCommandException* ex ) {
            // No errors logged when level is very low.
            if ( errorLevel != FdoXmlFlags::ErrorLevel_VeryLow )
                pContext->AddError( ex );
            ex->Release();
        }
    }
}

void FdoRdbmsOvClassDefinition::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    FdoInt32 idx;

    xmlWriter->WriteStartElement( L"complexType" );

    FdoPhysicalClassMapping::_writeXml( xmlWriter, flags );

    if ( mMappingType != FdoSmOvTableMappingType_Default ) {
        xmlWriter->WriteAttribute( L"tableMapping", FdoSmOvTableMappingTypeMapper::Type2String(mMappingType) );
    }

    _writeXmlAttributes(xmlWriter, flags);

    if ( mTable )
        mTable->_writeXml( xmlWriter, flags );

    /* For Future
    for ( idx = 0; idx < mUniqueKeys->GetCount(); idx++ )
        FdoOracleOvUniqueKeyP(mUniqueKeys->GetItem(idx))->_writeXml( xmlWriter, flags );

    for ( idx = 0; idx < mIndices->GetCount(); idx++ )
        FdoOracleOvIndexP(mIndices->GetItem(idx))->_writeXml( xmlWriter, flags );
*/
    for ( idx = 0; idx < mProperties->GetCount(); idx++ )
        FdoRdbmsOvPropertyP(mProperties->GetItem(idx))->_writeXml( xmlWriter, flags );

    xmlWriter->WriteEndElement();
}

void FdoRdbmsOvClassDefinition::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    // this method is to be overridden by subclasses, if required
}


FdoXmlSaxHandler* FdoRdbmsOvClassDefinition::XmlStartElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname,
    FdoXmlAttributeCollection* atts
)
{
    // Find out of this is a child of an Object Property Mapping type.
    // If it is then log errors against the Property Mapping object
    // instead of this.
    FdoPhysicalElementMappingP errRptClass = GetParent();
    FdoStringP errElemName = L"PropertyMapping";

    FdoRdbmsOvPropertyMappingDefinition* pObjPropMapping =
        dynamic_cast<FdoRdbmsOvPropertyMappingDefinition*>(errRptClass.p);

    if ( !pObjPropMapping ) {
        // Not a Property Mapping child, just log errors against this class.
        errElemName = L"complexType";
        errRptClass = FDO_SAFE_ADDREF(this);
    }

    // Handle generic sub-elements.
    FdoXmlSaxHandler* pRet = FdoPhysicalClassMapping::XmlStartElement(context, uri, name, qname, atts);

    if ( !pRet ) {
        // Not a generic sub-element so must be a class; handle it here.

        // Handle table override. mCurrAtts is only set when inside an
        // "element" sub-element.
        if ( (!mCurrAtts) && (wcscmp(name, L"Table") == 0) ) {
            if ( mTable ) {
                // Multiple table overrides not allowed.
                pRet = errRptClass->MultiSubElementError( context, errElemName, L"Table" );
            }
            else {
                SetTable( context, atts );
                pRet = mTable;
            }
        }

/* For future
        // Handle Unique Key override.

        if ( (!mCurrAtts) && (wcscmp(name, L"UniqueKey") == 0) ) {
            FdoOracleOvUniqueKeyP uniqueKey = FdoOracleOvUniqueKeyDefinition::Create();
            uniqueKey->InitFromXml( context, atts );
            mUniqueKeys->Add( uniqueKey );

            pRet = uniqueKey;
        }

        // Handle Index override.

        if ( (!mCurrAtts) && (wcscmp(name, L"Index") == 0) ) {
            FdoOracleOvIndexP index = FdoOracleOvIndexDefinition::Create();
            index->InitFromXml( context, atts );
            mIndices->Add( index );

            pRet = index;
        }
*/
        // Handle an element. We do not know the element's class
        // (Data, Object, or Geometric property) at this point. Therefore
        // this class also handles reading the element sub-elements. The element
        // attributes are saved in mCurrAtts. When we figure out the element class
        // then we create an object and initialize it from these attributes.
        // mCurrAtts also tells us whether we're processing a direct sub-element
        // of this class or an element (property) sub-element.
        if ( (wcscmp(name, L"element") == 0) ) {
            if (!mCurrAtts)
                mCurrAtts = FDO_SAFE_ADDREF(atts);
            if (mCurrAtts) {
                if (NULL != FdoPtr<FdoXmlAttribute>(mCurrAtts->FindItem(L"GeometricColumnType")) ||
                    NULL != FdoPtr<FdoXmlAttribute>(mCurrAtts->FindItem(L"GeometricContentType")) ||
                    NULL != FdoPtr<FdoXmlAttribute>(mCurrAtts->FindItem(L"xColumnName")) ||
                    NULL != FdoPtr<FdoXmlAttribute>(mCurrAtts->FindItem(L"yColumnName")) ||
                    NULL != FdoPtr<FdoXmlAttribute>(mCurrAtts->FindItem(L"zColumnName"))) {

                    // We now know that the current element represents a Geometric Property
                    // so create one.
                    mCurrGeomProp = CreateGeometricProperty( context, mCurrAtts, atts );
                    mCurrGeomProp->SetParent(this);
                    mCurrProp = mCurrGeomProp.p;
                    pRet = CheckDuplicateProperty( context, mCurrProp );

                    if ( !pRet ) {
                        AddProperty(mCurrProp);
                        pRet = (FdoRdbmsOvGeometricColumn*) mCurrProp;
                    }
                }

            }
            pRet = this;
        }

        // Handle Column sub-element of a property.
        if ( mCurrAtts && (wcscmp(name, L"Column") == 0) ) {
            if ( mCurrProp) {
                if ( mCurrDataProp )
                    // Multiple column overrides not allowed
                    pRet = mCurrProp->MultiSubElementError( context, L"element", L"Column" );
                else
                    // Column overrides only allowed on data properties.
                    pRet = mCurrProp->ChoiceSubElementError( context, L"element", mElementSubElements );
            }
            else {
                // We now know that the current element represents a Data Property
                // so create one.
                mCurrDataProp = CreateDataProperty( context, mCurrAtts, atts );
                mCurrDataProp->SetParent(this);
                mCurrProp = mCurrDataProp.p;
                pRet = CheckDuplicateProperty( context, mCurrProp );

                if ( !pRet )  {
                    AddProperty(mCurrProp);
                    FdoRdbmsOvColumnP pCol = mCurrDataProp->GetColumn();
                    pRet = (FdoRdbmsOvColumn*) pCol;
                }
            }
        }

        // Handle GeometricColumn sub-element of a property.
        if ( mCurrAtts && (wcscmp(name, L"GeometricColumn") == 0) ) {
            if ( mCurrProp) {
                if ( mCurrGeomProp ) {
                    FdoRdbmsOvGeometricColumnP pGeomCol = mCurrGeomProp->GetColumn();
                    if (pGeomCol != NULL) {
                        // Multiple geometric column overrides not allowed
                        pRet = mCurrProp->MultiSubElementError( context, L"element", L"GeometricColumn" );
                    }
                } else {
                    // Geometric Column overrides only allowed on geometric properties.
                    pRet = mCurrProp->ChoiceSubElementError( context, L"element", mElementSubElements );
                }
            }
            else {
                // We now know that the current element represents a Geometric Property
                // so create one.
                mCurrGeomProp = CreateGeometricProperty( context, mCurrAtts, atts );
                mCurrGeomProp->SetParent(this);
                mCurrProp = mCurrGeomProp.p;
                pRet = CheckDuplicateProperty( context, mCurrProp );

                if ( !pRet ) {
                    AddProperty(mCurrProp);
                    FdoRdbmsOvGeometricColumnP pGeomCol = mCurrGeomProp->GetColumn();
                    pRet = (FdoRdbmsOvGeometricColumn*) pGeomCol;
                }
            }
        }

        // Handle any Object Property Mapping sub-element of a property.
        if ( mCurrAtts && (mOpSubElements->IndexOf(name) > -1) ) {
            if ( mCurrProp) {
                if ( mCurrObjProp )
                    // Multiple object property mapping overrides not allowed
                    pRet = mCurrProp->MultiSubElementError( context, L"element", L"PropertyMapping" );
                else
                    // Object property mapping overrides only allowed on object properties.
                    pRet = mCurrProp->ChoiceSubElementError( context, L"element", mElementSubElements );
            }
            else {
                // We now know that the current element represents an Object Property
                // so create one.
                mCurrObjProp = CreateObjectProperty( context, mCurrAtts, name, atts );
                mCurrObjProp->SetParent(this);
                mCurrProp = mCurrObjProp.p;

                pRet = CheckDuplicateProperty( context, mCurrProp );

                if ( !pRet ) {
                    AddProperty(mCurrProp);
                    FdoRdbmsOvPropertyMappingP pMapping = mCurrObjProp->GetMappingDefinition();
                    pRet = (FdoRdbmsOvPropertyMappingDefinition*) pMapping;
                }
            }
        }
    }

    if ( !pRet ) {
        // If no SAX Handler has been set by now, then the current sub-element
        // is not allowed inside a complexType. Log an error.
        if ( mCurrProp ) {
            // Inside an "element", log against the element.
            pRet = mCurrProp->SubElementError( context, L"element", name );
        }
        else {
            if ( mCurrAtts ) {
                // Inside an "element" whose object has not yet been created,
                // create a temporary object and log against it.
                FdoRdbmsOvDataPropertyP pProp = CreateDataProperty( context, mCurrAtts, atts);
                pProp->SetParent( this );
                pRet = pProp->SubElementError( context, L"element", name );
            }
            else {
                // Log against this object (or containing Property Mapping if
                // contained by an ObjectProperty Mapping_.
                pRet = errRptClass->SubElementError( context, errElemName, name );
            }
        }
    }

    return(pRet);
}

FdoBoolean FdoRdbmsOvClassDefinition::XmlEndElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname
)
{
    if ( wcscmp(name, L"element") == 0 ) {
        // At end of element so clear out all current property override indicators.
        mCurrAtts = NULL;
        mCurrProp = NULL;
        mCurrDataProp = NULL;
        mCurrGeomProp = NULL;
        mCurrObjProp = NULL;
    }

    return FdoPhysicalClassMapping::XmlEndElement( context, uri, name, qname );
}

FdoRdbmsOvPropertyDefinitionCollection *FdoRdbmsOvClassDefinition::GetRdbmsProperties()
{
    return FDO_SAFE_ADDREF(mProperties.p);
}

void FdoRdbmsOvClassDefinition::SetTable(FdoRdbmsOvTable *table)
{
    mTable = FDO_SAFE_ADDREF(table);
    mTable->SetParent(this);
}

FdoXmlSaxHandler* FdoRdbmsOvClassDefinition::CheckDuplicateProperty( 
    FdoXmlSaxContext* context,
    FdoRdbmsOvPropertyDefinition* pProp 
)
{
    FdoXmlSaxHandler* pRet = NULL;
    FdoRdbmsOvPropertyP pDupProp = mProperties->FindItem( pProp->GetName() );

    if ( pDupProp ) {
        // Set parent so that right error message is generated.
        pProp->SetParent(this);
        pRet = DuplicateSubElementError( context, L"complexType", L"element", pProp->GetName() );
    }

    return pRet;
}
